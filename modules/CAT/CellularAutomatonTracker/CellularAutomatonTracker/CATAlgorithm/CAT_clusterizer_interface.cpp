/* -*- mode: c++ -*- */
// CAT_clusterizer_interface.cpp

#include <CATAlgorithm/tracked_data.h>
#include <stdexcept>
#include <limits>
#include <sstream>
#include <CATAlgorithm/CAT_clusterizer_interface.h>
#include <boost/algorithm/string.hpp>

namespace CAT {

  const std::string & clusterizer_setup_data::get_error_message () const
  {
    return _error_message;
  }

  void clusterizer_setup_data::_set_error_message (const std::string & message_)
  {
    std::ostringstream oss;
    oss << "CAT::clusterizer_setup_data: ";
    oss << message_;
    _error_message = oss.str ();
    return;
  }

  clusterizer_setup_data::clusterizer_setup_data ()
  {
    _set_defaults ();
    return;
  }

  void clusterizer_setup_data::reset ()
  {
    _set_defaults ();
    return;    
  }

  void clusterizer_setup_data::_set_defaults ()
  {
    _error_message.clear ();
    level         = "normal";
    SuperNemo     = true;
    MaxTime       = 5000.  * CLHEP::ms;  
    SmallRadius   =    0.1 * CLHEP::mm; 
    TangentPhi    =   20.  *  CLHEP::degree; 
    TangentTheta  =  160.  * CLHEP::degree;
    SmallNumber   =    0.1 * CLHEP::mm;
    QuadrantAngle =   90.  *  CLHEP::degree;
    Ratio         =   10.;
    CompatibilityDistance = 4.0 * CLHEP::mm;
    MaxChi2       = 3.;
    nsigma        = 4.;
    nofflayers    = 1;

    // SuperNEMO geometry default parameters :
    num_blocks = 1;
    planes_per_block.clear ();
    planes_per_block.push_back (num_blocks);
    planes_per_block.at(0) = 9;
    num_cells_per_plane    = 113;
    cell_size            =   44.0  * CLHEP::mm;
 
    return;
  }

  bool clusterizer_setup_data::check () const
  {
    clusterizer_setup_data * mutable_this = const_cast<clusterizer_setup_data *> (this);
    return mutable_this->_check_snemo ();
  }

  bool clusterizer_setup_data::_check_snemo ()
  {
    if (SmallRadius <= 0.0) 
      {
        _set_error_message ("Invalid 'SmallRadius'"); 
        return false;
      }
    if (TangentPhi <= 0.0) 
      {
        _set_error_message ("Invalid 'TangentPhi'"); 
        return false;
      }
    if (TangentTheta <= 0.0) 
      {
        _set_error_message ("Invalid 'TangentTheta'"); 
        return false;
      }
    if (SmallNumber <= 0.0) 
      {
        _set_error_message ("Invalid 'SmallNumber'"); 
        return false;
      }
    if (QuadrantAngle <= 0.0) 
      {
        _set_error_message ("Invalid 'QuadrantAngle'"); 
        return false;
      }
    if (Ratio <= 0.0) 
      {
        _set_error_message ("Invalid 'Ratio'"); 
        return false;
      }
    if (CompatibilityDistance <= 0.0)
      {
        _set_error_message ("Invalid 'CompatibilityDistance'"); 
        return false;
      }
    if (MaxChi2 <= 0.0) 
      {
        _set_error_message ("Invalid 'MaxChi2'"); 
        return false;
      }
    if (nsigma <= 0.0) 
      {
        _set_error_message ("Invalid 'nsigma'"); 
        return false;
      }
    if (nofflayers < 0.0) 
      {
        _set_error_message ("Invalid 'nofflayers'"); 
        return false;
      }
    if (num_blocks < 1) 
      {
        _set_error_message ("Invalid 'num_blocks'"); 
        return false;
      }
    if (planes_per_block.size () != num_blocks)
      {
        _set_error_message ("Invalid size of 'planes_per_block'"); 
        return false;
      }
    if (num_cells_per_plane < 1) 
      {
        _set_error_message ("Invalid 'num_cells_per_plane'"); 
        return false;
      }
    if (cell_size <= 0.0) 
      {
        _set_error_message ("Invalid 'cell_size'"); 
        return false;
      }

    return true;
  }

  void clusterizer_configure (clusterizer & czer_, 
                              const clusterizer_setup_data & setup_)
    {
      if (! setup_.check ())
        {
          std::cerr << "ERROR: CAT::clusterizer_configure: " 
                    << setup_.get_error_message () << std::endl;
          throw std::logic_error ("CAT::clusterizer_configure: Invalid setup data !");
        }

      // General parameters :
      czer_.set_PrintMode (false);
      czer_.set_MaxTime (setup_.MaxTime / CLHEP::ms);
      string leveltmp = setup_.level;
      boost::to_upper(leveltmp);
      czer_.set_level (leveltmp); //mybhep::get_info_level (leveltmp));

      // Algorithm parameters :
      czer_.set_SmallRadius (setup_.SmallRadius / CLHEP::mm);
      czer_.set_TangentPhi (setup_.TangentPhi / CLHEP::degree);
      czer_.set_TangentTheta (setup_.TangentTheta / CLHEP::degree);
      czer_.set_SmallNumber (setup_.SmallNumber / CLHEP::mm);
      czer_.set_QuadrantAngle (setup_.QuadrantAngle / CLHEP::degree);
      czer_.set_Ratio (setup_.Ratio);
      czer_.set_CompatibilityDistance (setup_.CompatibilityDistance);
      czer_.set_MaxChi2 (setup_.MaxChi2);
      czer_.set_nsigma (setup_.nsigma);
      czer_.set_nofflayers (setup_.nofflayers);

      // Geometry description :
      if (setup_.SuperNemo)
        {
          
          /// Activate the special new mode :
          czer_.set_SuperNemoChannel(true);

          // Layout of the tracking chamber : 
          czer_.set_num_blocks (setup_.num_blocks);
          for (int i = 0; i < setup_.num_blocks; i++)
            {
              czer_.set_planes_per_block (i, setup_.planes_per_block.at (i)); 
            }
          czer_.set_num_cells_per_plane (setup_.num_cells_per_plane);
          czer_.set_GG_CELL_pitch (setup_.cell_size / CLHEP::mm);
        }
      else
        {
          throw std::logic_error ("CAT::clusterizer_configure: Only SuperNEMO setup is supported !");
        }

      return;
    }


    /***********************************************************/

    topology::cell & clusterizer_input_data::add_cell ()
    {
      if (cells.size () == 0)
        {
          // memory preallocation at the first cell  
          cells.reserve (50);
        }
      {
        topology::cell tmp;
        cells.push_back (tmp);
      }
      return cells.back ();
    }

    clusterizer_input_data::clusterizer_input_data ()
    {
      return;
    }

    /***********************************************************/

    clusterizer_output_data::clusterizer_output_data ()
    {
      return;
    }

  }

  // end of CAT_clusterizer_interface.cpp

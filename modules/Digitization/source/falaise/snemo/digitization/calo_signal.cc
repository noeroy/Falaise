// snemo/digitization/calo_signal.cc
// Author(s): Yves LEMIERE <lemiere@lpccaen.in2p3.fr>
// Author(s): Guillaume OLIVIERO <goliviero@lpccaen.in2p3.fr>

// Ourselves:
#include <snemo/digitization/calo_signal.h>

// Third party:
// - Bayeux/datatools:
#include <datatools/exception.h>
#include <datatools/utils.h>

namespace snemo {
  
  namespace digitization {

    // Serial tag for datatools::serialization::i_serializable interface :
    // DATATOOLS_SERIALIZATION_SERIAL_TAG_IMPLEMENTATION(calo_signal, "snemo::digitalization::calo_signal")
    
    calo_signal::calo_signal()
    {
      _locked_ = false;
      datatools::invalidate(_amplitude_);
      return;
    }

    calo_signal::~calo_signal()
    {
      reset();
      return;
    }

    double calo_signal::get_signal_time() const
    {
      return _signal_time_;
    }
    
    void calo_signal::set_signal_time(const double & signal_time_)
    {
      DT_THROW_IF(is_locked(), std::logic_error, "Calo signal is locked ! ");
      _signal_time_ = signal_time_ + DELAYED_PM_TIME;
      return;
    }
   
    double calo_signal::get_amplitude() const
    {
      return _amplitude_;
    }
    
    void calo_signal::set_amplitude(const double & amplitude_)
    {
      DT_THROW_IF(is_locked(), std::logic_error, "Calo signal is locked ! ");
      _amplitude_ = amplitude_;
      return;
    }

    bool calo_signal::is_locked() const
    {
      return _locked_;
    }

    void calo_signal::lock()
    {
      DT_THROW_IF(is_locked(), std::logic_error, "Calo signal is already locked ! ");
      _check();
      _locked_ = true;
      return;
    }
    
    void calo_signal::unlock()
    {
      DT_THROW_IF(!is_locked(), std::logic_error, "Calo signal is already unlocked ! ");
      _locked_ = false;
      return;
    } 

    bool calo_signal::is_valid() const
    {
      return true;
    }

    void calo_signal::reset()
    {
      if(is_locked())
	{
	  unlock();
	}
      geomtools::base_hit::reset();
      return;
    }

    void calo_signal::tree_dump (std::ostream & out_,
				   const std::string & title_,
				   const std::string & indent_,
				   bool inherit_) const
    {
      base_hit::tree_dump (out_, title_, indent_, true);

      out_ << indent_ << datatools::i_tree_dumpable::tag
           << "Amplitude =  : " << _amplitude_ * CLHEP::megaelectronvolt << " MeV " << std::endl;

      return;
    }
    
    void calo_signal::_check()
    {
      DT_THROW_IF(!is_valid(), std::logic_error, " is not valid ! ");
    }


  } // end of namespace digitization

} // end of namespace snemo

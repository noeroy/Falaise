/* -*- mode: c++ -*- */
#ifndef __CATAlgorithm__ILOGICSCENARIO
#define __CATAlgorithm__ILOGICSCENARIO
#include <iostream>
#include <cmath>
#include <mybhep/error.h>
#include <mybhep/utilities.h>
#include <mybhep/point.h>
#include <mybhep/hit.h>
#include <mybhep/system_of_units.h>
#include <boost/cstdint.hpp>

#include <CATAlgorithm/tracking_object.h>
#include <CATAlgorithm/experimental_point.h>
#include <CATAlgorithm/experimental_vector.h>
#include <CATAlgorithm/sequence.h>
#include <CATAlgorithm/node.h>
#include <CATAlgorithm/calorimeter_hit.h>
#include <CATAlgorithm/scenario.h>
#include <CATAlgorithm/logic_cell.h>
#include <CATAlgorithm/logic_sequence.h>


namespace CAT {
namespace topology{

using namespace std;
using namespace mybhep;

  class logic_scenario : public tracking_object{

    // a reconstruction scenario is composed of 
    // a collection of logic_sequences

  private:
    // chi2
    double chi2_;
    int32_t ndof_;

    // n of free families
    size_t n_free_families_;

    // n of overlapping cells
    size_t n_overlaps_;

  public:   

    // tracks
    std::vector<topology::logic_sequence> sequences_;

    //!Default constructor 
    logic_scenario()
    {
      sequences_.clear();
      chi2_ = small_neg;
      ndof_ = default_integer;
      n_free_families_ = default_integer;
      n_overlaps_ = default_integer;
   }

    //!Default destructor
    virtual ~logic_scenario(){};

    //! constructor
    logic_scenario(topology::scenario sc){
      for(std::vector<topology::sequence>::iterator iseq=sc.sequences_.begin(); iseq!=sc.sequences_.end(); ++iseq)
        sequences_.push_back(logic_sequence(*iseq));
      chi2_ = sc.chi2();
      ndof_ = sc.ndof();
      n_free_families_ = sc.n_free_families();
      n_overlaps_ = sc.n_overlaps();
    }

    //! get sequences
    const std::vector<logic_sequence> & sequences()const
    {
      return sequences_;
    }      

    //!get chi2
    const double& chi2() const 
    {
      return chi2_;
    } 

    //!get ndof
    const int32_t& ndof() const 
    {
      return ndof_;
    } 

    //!get n free families
    const size_t& n_free_families() const {return n_free_families_;} 

    //!get n overlaps
    const size_t& n_overlaps() const {return n_overlaps_;} 


    void calculate_n_overlaps(std::vector<topology::cell> cells, std::vector<topology::calorimeter_hit> calos){

      std::vector<int> freecells(cells.size());
      fill(freecells.begin(), freecells.end(), 1);

      std::vector<int> freecalos(calos.size());
      fill(freecalos.begin(), freecalos.end(), 1);

      size_t counter = 0;

      for(std::vector<logic_sequence>::iterator iseq = sequences_.begin(); iseq != sequences_.end(); ++iseq){

        for(std::vector<logic_cell>::iterator in = iseq->cells_.begin(); in != iseq->cells_.end(); ++in){
          if( in->id() >= cells.size() ){
            if( print_level() >= mybhep::VVERBOSE )
              clog << " problem: cell " << in->id() << " has larger id than n of cells " << cells.size() << endl;
            continue;
          }

          if( freecells[in->id()] )
            freecells[in->id()] = 0;
          else
            counter ++;

        }

        if( iseq->has_decay_vertex() ){
          if( iseq->calo_id() >= calos.size() ){
            if( print_level() >= mybhep::VVERBOSE )
              clog << " problem: calo " << iseq->calo_id() << " has larger id than n of calos " << calos.size() << endl;
            continue;
          }

          if( freecalos[iseq->calo_id()] )
            freecalos[iseq->calo_id()] = 0;
          else
            counter ++;

        }
        
      }
    

      n_overlaps_ = counter;

      return;
      
    }


    void calculate_n_free_families(std::vector<topology::cell> cells, std::vector<topology::calorimeter_hit> calos){

      std::vector<int> freecells(cells.size());
      fill(freecells.begin(), freecells.end(), 1);

      std::vector<int> freecalos(calos.size());
      fill(freecalos.begin(), freecalos.end(), 1);


      for(std::vector<logic_sequence>::iterator iseq = sequences_.begin(); iseq != sequences_.end(); ++iseq){

        for(std::vector<logic_cell>::iterator in = iseq->cells_.begin(); in != iseq->cells_.end(); ++in){
          if( in->id() >= cells.size() ){
            if( print_level() >= mybhep::VVERBOSE )
              clog << " problem: cell " << in->id() << " has larger id than n of cells " << cells.size() << endl;
            continue;
          }
          else{
            freecells[in->id()] = 0;
          }
        }

        if( !iseq->has_decay_vertex() ) continue;

        if( iseq->calo_id() >= calos.size() ){
          if( print_level() >= mybhep::VVERBOSE )
            clog << " problem: calo " << iseq->calo_id() << " has larger id than n of calos " << calos.size() << endl;
          continue;
        }
        freecalos[iseq->calo_id()] = 0;

      }

      size_t counter = 0;
      for(std::vector<int>::iterator i=freecells.begin(); i!= freecells.end(); ++i)
        if( *i )
          counter ++;

      for(std::vector<int>::iterator i=freecalos.begin(); i!= freecalos.end(); ++i)
        if( *i )
          counter ++;

      n_free_families_ = counter;

      return;
    }


    void calculate_chi2(){

      double chi2 = 0.;
      int32_t ndof = 0;
      for(std::vector<logic_sequence>::iterator iseq = sequences_.begin(); iseq != sequences_.end(); ++iseq){
        chi2 += iseq->chi2();
        ndof += iseq->ndof();
      }

      chi2_ = chi2;
      ndof_ = ndof;

      return;
    }


    double Prob(){
      return probof(chi2(), ndof());
    }

    bool better_scenario_than( logic_scenario s){

      // - n of recovered cells 
      int deltanfree = n_free_families() - s.n_free_families();

      // n of new overlaps 
      int deltanoverls = n_overlaps() - s.n_overlaps();

      double deltaprob = Prob() - s.Prob();


      if( print_level() >= mybhep::VVERBOSE ){
        clog << " delta n_free_families = (" << n_free_families()  << " - " << s.n_free_families() << ")= " << deltanfree 
             << " dela n_overlaps = (" << n_overlaps() << " - " << s.n_overlaps() << ")= " << deltanoverls
             << " delta prob = (" << Prob()  << " - " << s.Prob() << ") = " << deltaprob << endl;
      }

      if( deltanoverls < - 2*deltanfree )
        return true;

      if( deltanoverls == - 2*deltanfree ){

        if( deltaprob > 0. )
            return true;
      }

      return false;
    }

  };
}
}

#endif

// test_snemo_datamodel_sim_digi_data.cxx

// Standard libraries :
#include <iostream>
#include <exception>
#include <cstdlib>

// Third party:
// - Bayeux/datatools:
#include <bayeux/datatools/logger.h>
#include <bayeux/datatools/io_factory.h>
#include <bayeux/datatools/clhep_units.h>
#include <bayeux/datatools/things.h>
// - Bayeux/mctools:
#include <bayeux/mctools/simulated_data.h>
#include <bayeux/mctools/signal/signal_data.h>

// Falaise:
#include <falaise/falaise.h>

// This project :
#include <snemo/datamodels/event_header.h>
#include <snemo/datamodels/sim_digi_data.h>
#include <snemo/datamodels/sim_calo_digi_hit.h>
#include <snemo/datamodels/sim_tracker_digi_hit.h>

void test1();
void test2();

int main(int argc_, char ** argv_)
{
  falaise::initialize(argc_, argv_);
  int error_code = EXIT_SUCCESS;
  datatools::logger::priority logging = datatools::logger::PRIO_FATAL;
  long int seed = 314159;
  srand48(seed);

  try {
    std::clog << "Test program for class 'snemo::datamodel::sim_digi_data' !" << std::endl;

    test1();
    test2();

    std::clog << "The end." << std::endl;
  } catch (std::exception & error) {
    DT_LOG_FATAL(logging, error.what());
    error_code = EXIT_FAILURE;
  } catch (...) {
    DT_LOG_FATAL(logging, "Unexpected error!");
    error_code = EXIT_FAILURE;
  }
  falaise::terminate();
  return error_code;
}

void test1()
{
  std::clog << "\ntest1: running...\n";
  double gigahertz = 1.e9 * CLHEP::hertz;

  // Create and set some dummy data
  snemo::datamodel::sim_digi_data sddWrite;
  // Calling back() on empty vector is *undefined behaviour*
  // so must put one event into sim_digi_data instance before
  // the loop can proceed
  auto& eventCollection = sddWrite.grab_digi_events();
  // Unreadable, but just a push back of a new handle-to-data.
  // Note that sim_digi_data provides a "handle_type" typedef, but not the
  // underlying type. We could derive that from the "handle_type",
  // but that would almost double the number of characters...
  eventCollection.push_back(snemo::datamodel::sim_digi_data::sim_digi_event_handle_type{new snemo::datamodel::sim_digi_event_data});

  for (std::size_t icalohit = 0; icalohit < 3; icalohit++) {
    snemo::datamodel::sim_calo_digi_hit dummy;
    // That's way, way too many leveks of nesting...
    sddWrite.grab_digi_events().back().grab().grab_readout_data().grab_calo_digi_hits().push_back(dummy);

    // .... ...
    snemo::datamodel::sim_calo_digi_hit & calo_hit = sddWrite.grab_digi_events().back().grab().grab_readout_data().grab_calo_digi_hits().back();
    calo_hit.set_hit_id(icalohit);
    calo_hit.grab_geom_id().set_type(1234);
    calo_hit.grab_geom_id().set_address(1, 3, 8 - icalohit, 3 + icalohit);
    calo_hit.set_sampling_frequency(1.0 * gigahertz);
    calo_hit.set_number_of_samples(128, 0);
  }

  // Write the data to bare xml file
  sddWrite.tree_dump(std::clog, "Simulated Digitized Data: ");
  datatools::data_writer writer("test_snemo_datamodel_sim_digi_data.xml",
                                datatools::using_multi_archives);
  writer.store(sddWrite);

  // Create and read back data
  snemo::datamodel::sim_digi_data sddReadback;
  datatools::data_reader reader("test_snemo_datamodel_sim_digi_data.xml",
                                datatools::using_multi_archives);
  reader.load(sddReadback);
  sddReadback.tree_dump(std::clog, "Loaded Simulated Digitized Data: ");
}

void test2()
{
  std::clog << "\ntest2: running...\n";

  {
    datatools::things event_record;

    // Add event header bank:
    snemo::datamodel::event_header & eh =
      event_record.add<snemo::datamodel::event_header>("EH");
    eh.grab_id().set_run_number(12);
    eh.grab_id().set_event_number(654);
    eh.grab_properties().store("test", "foo");
    eh.set_generation(snemo::datamodel::event_header::GENERATION_SIMULATED);

    // Add simulated data bank:
    event_record.add<mctools::simulated_data>("SD");

    // Add simulated signal data bank:
    event_record.add<mctools::signal::signal_data>("SSD");

    // Add simulated digitized data bank:
    event_record.add<snemo::datamodel::sim_digi_data>("SDD");

    event_record.tree_dump(std::clog, "Event record: ");

    {
      std::clog << "Serialization..." << std::endl;
      datatools::data_writer writer("test_snemo_datamodel_sim_digi_data2.xml",
                                    datatools::using_multi_archives);
      writer.store(event_record);
    }

  }

  {
    datatools::things event_record;
    {
      std::clog << "Deserialization..." << std::endl;
      datatools::data_reader reader("test_snemo_datamodel_sim_digi_data2.xml",
                                    datatools::using_multi_archives);
      reader.load(event_record);
    }
    event_record.tree_dump(std::clog, "Event record: ");
  }

  return;
}

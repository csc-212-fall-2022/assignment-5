#include "Election.hpp"
#include "ballot.hpp"
#include "boost/program_options.hpp"
#include <filesystem>
#include <iostream>

namespace po = boost::program_options;
namespace fs = std::filesystem;

using namespace assignment5;

void run_elections(ElectionData data) {
  for (const auto &contest : data.contests) {
    std::cout << "Election " << contest.name << std::endl;
    auto election = Election(contest, data.candidates.at(contest.id),
                             data.ballots.at(contest.id));
    election.run();
  }
}

int main(int ac, char *av[]) {
  po::options_description desc("Options");
  desc.add_options()("help", "print this message")(
      "data-dir", po::value<std::string>(), "directory with downloaded data");

  po::variables_map vm;
  po::store(po::parse_command_line(ac, av, desc), vm);
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << desc << std::endl;
    return 1;
  }

  if (vm.count("data-dir")) {
    std::string data_dir_s = vm["data-dir"].as<std::string>();
    fs::path data_dir(data_dir_s);
    fs::path bi_fname("20161206_ballotimage.txt");
    fs::path ml_fname("20161206_masterlookup.txt");

    fs::path ballotimage_path = data_dir / bi_fname;
    fs::path masterlookup_path = data_dir / ml_fname;

    std::cout << "Loading election data..." << std::endl;

    ElectionData election_data =
        LoadElectionData(ballotimage_path, masterlookup_path);
    run_elections(election_data);
  } else {
    std::cout << "--data-dir={path to data directory} must be passed!"
              << std::endl;
    return 1;
  }

  return 0;
}
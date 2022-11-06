#include "Election.hpp"
#include "ballot.hpp"
#include "boost/format.hpp"
#include "boost/program_options.hpp"
#include <filesystem>
#include <iostream>

namespace po = boost::program_options;
namespace fs = std::filesystem;

using namespace assignment5;

void run_elections(ElectionData data, std::optional<int> district) {
  for (const auto &contest : data.contests) {
    if (district.has_value() && contest.district != district.value()) {
      continue;
    }
    std::cout<< "****Election " << contest.name << " ****" << std::endl;
    auto election = Election(contest, data.candidates.at(contest.id), data.ballots.at(contest.id));
    election.run();
  }
}

int main(int ac, char *av[]) {
  const std::unordered_set<int> districts {1,3,5,7,9,11};

  po::options_description desc("Options");
  desc.add_options()("help", "print this message")(
      "data-dir", po::value<std::string>(), "directory with downloaded data")(
      "district", po::value<int>(), ""
      );

  po::variables_map vm;
  po::store(po::parse_command_line(ac, av, desc), vm);
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << desc << std::endl;
    return 1;
  }

  std::optional<int> district = std::nullopt;

  if (vm.count("district")) {
    int d = vm["district"].as<int>();
    if(!districts.count(d)){
      std::cout << boost::format("Only valid districts are 1, 3, 7, 9, 11. Received %s") % d << std::endl;
    }
    district = d;
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
    run_elections(election_data, district);
  } else {
    std::cout << "--data-dir={path to data directory} must be passed!"
              << std::endl;
    return 1;
  }

  return 0;
}

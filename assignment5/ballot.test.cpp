#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "ballot.hpp"
#include "doctest/doctest.h"
using namespace assignment5;

TEST_CASE("Test ballot master parse candidate") {
  std::string line = "Candidate 0000121SAMUEL KWONG                            "
                     "          0000001000000900";
  auto result = ParseElectionMasterLine(line);
  CHECK(result.has_value());
  CHECK_EQ(std::get<Candidate>(result.value()),
           Candidate{"SAMUEL KWONG", 121, 9});
}

TEST_CASE("Test ballot master parse contest") {
  std::string line = "Contest   0000011Board of Supervisors, District 5        "
                     "          0000014000000000";
  auto result = ParseElectionMasterLine(line);

  CHECK(result.has_value());
  CHECK_EQ(std::get<Contest>(result.value()),
           Contest{"Board of Supervisors, District 5", 11});
}

TEST_CASE("Test ballot master parse ñ properly") {
  std::string line = "Candidate 0000142ISWARI ESPAÑA                           "
                     "          0000003000001300";
  auto result = ParseElectionMasterLine(line);

  CHECK(result.has_value());
  Candidate candidate = std::get<Candidate>(result.value());
  CHECK_EQ(candidate.name, "ISWARI ESPAÑA");
  CHECK_EQ(candidate.id, 142);
  CHECK_EQ(std::get<Candidate>(result.value()),
           Candidate{"ISWARI ESPAÑA", 142, 13});
}
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "CandidateVotes.hpp"
#include "ElectionQueue.hpp"
#include "doctest/doctest.h"

namespace assignment5 {
TEST_CASE("Test max heapify") {
  Ballot ballot = Ballot();
  std::vector<Ballot> ballot_one{ballot};
  std::vector<Ballot> ballot_two{ballot, ballot};
  Candidate candidate = Candidate{"foo", 1, 1};
  CandidateVotes one_vote = CandidateVotes(candidate, ballot_one);
  CandidateVotes two_votes = CandidateVotes(candidate, ballot_two);

  std::vector<CandidateVotes> vec{one_vote, two_votes};
  ElectionQueue::max_heapify(vec);
  CHECK_EQ(vec.at(0), two_votes);
}

} // namespace assignment5
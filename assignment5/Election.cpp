
#include "Election.hpp"
#include "boost/format.hpp"
#include <iostream>

namespace assignment5 {
Election::Election(const Contest &contest,
                   const std::vector<Candidate> &candidates,
                   const std::vector<Ballot> &ballots) {
  this->contest = contest;
  queue = ElectionQueue(candidates, ballots);
}

void Election::step() {
  // if we have a winner, we're done
  if (queue.hasWinner()) {
    Candidate winner = queue.winner();
    std::cout << boost::format("%s has won %s") % winner.name % contest.name
              << std::endl;
  } else {
    EliminationInfo elimination = queue.eliminate();
    std::cout << boost::format("%s has been eliminated. %s ballots exhausted") %
                     elimination.eliminatedCandidate.name %
                     elimination.exhaustedBallots
              << std::endl;
  }
}

void Election::run() {
  int round = 1;
  while (!queue.hasWinner()) {
    std::cout << "Round " << round << std::endl;
    step();
    round++;
  }
  // then, when we have a winner, one last step to display them
  std::cout << "Round " << round << std::endl;
  step();
}
} // namespace assignment5
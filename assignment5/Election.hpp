#ifndef ASSIGNMENT_5_ELECTION_HPP
#define ASSIGNMENT_5_ELECTION_HPP

#include "ElectionQueue.hpp"

namespace assignment5 {
class Election {
private:
  Contest contest;
  ElectionQueue queue;

public:
  Election(const Contest &, const std::vector<Candidate> &,
           const std::vector<Ballot> &);
  void step();
  void run();
};
} // namespace assignment5

#endif
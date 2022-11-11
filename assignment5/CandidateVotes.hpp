#ifndef ASSIGNMENT_5_CANDIDATEVOTES_HPP
#define ASSIGNMENT_5_CANDIDATEVOTES_HPP

#include "ballot.hpp"

namespace assignment5 {

// Just exists so we have a container for a candidate and their ballots
class CandidateVotes {
private:
  Candidate candidate_;
  // active ballots with candidate_ as their top choice
  std::vector<Ballot> ballots_;

public:
  explicit CandidateVotes(const Candidate &);
  CandidateVotes(const Candidate &, std::vector<Ballot>);
  ulong votes() const;
  void add_ballot(const Ballot &);
  int candidateId() const;
  std::vector<Ballot> &ballots();

  bool operator<(const CandidateVotes &) const;
  bool operator>(const CandidateVotes &) const;
  bool operator==(const CandidateVotes &) const;
};
} // namespace assignment5
#endif // ASSIGNMENT_5_CANDIDATEVOTES_HPP

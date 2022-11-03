#include "CandidateVotes.hpp"

namespace assignment5 {

CandidateVotes::CandidateVotes(const Candidate &cand) { candidate_ = cand; }

CandidateVotes::CandidateVotes(const Candidate &cand, std::vector<Ballot> bs) {
  candidate_ = cand;
  ballots_ = bs;
}

int CandidateVotes::candidateId() const { return candidate_.id; }

ulong CandidateVotes::votes() const { return ballots_.size(); }

std::vector<Ballot> &CandidateVotes::ballots() { return ballots_; }

void CandidateVotes::add_ballot(const Ballot &ballot) {
  ballots_.push_back(ballot);
}

bool CandidateVotes::operator<(const CandidateVotes &rhs) const {
  if (votes() == rhs.votes()) {
    return candidate_ < rhs.candidate_;
  }
  return votes() < rhs.votes();
}

bool CandidateVotes::operator>(const CandidateVotes &rhs) const {
  if (votes() == rhs.votes()) {
    return candidate_ > rhs.candidate_;
  }
  return votes() > rhs.votes();
}

bool CandidateVotes::operator==(const CandidateVotes &rhs) const {
  return candidate_ == rhs.candidate_ && ballots_ == rhs.ballots_;
}

} // namespace assignment5
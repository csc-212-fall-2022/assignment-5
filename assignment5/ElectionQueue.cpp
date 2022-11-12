#include "ElectionQueue.hpp"
#include "CandidateVotes.hpp"
#include <algorithm>
#include <functional>
#include <stdexcept>

namespace assignment5 {
ElectionQueue::ElectionQueue(const std::vector<Candidate> &candidates,
                             const std::vector<Ballot> &ballots) {
  throw std::runtime_error("Implement me!");
}

int ElectionQueue::numCandidates() { return minVoteHeap.size(); }

bool ElectionQueue::hasWinner() { throw std::runtime_error("Implement me!"); }

Candidate &ElectionQueue::winner() { throw std::runtime_error("Implement me!"); }

EliminationInfo ElectionQueue::eliminate() {
  throw std::runtime_error("Implement me!");
}

void ElectionQueue::max_heapify(std::vector<CandidateVotes> &vec) {
  std::make_heap(vec.begin(), vec.end());
}

void ElectionQueue::min_heapify(std::vector<CandidateVotes> &vec) {
  std::make_heap(vec.begin(), vec.end(), std::greater<>{});
}

} // namespace assignment5
#ifndef ASSIGNMENT_5_ELECTION_QUEUE_HPP
#define ASSIGNMENT_5_ELECTION_QUEUE_HPP

#include "CandidateVotes.hpp"
#include "ballot.hpp"
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace assignment5 {

// Helper function to group the elements of a vector according to some function.
// For example:
//  std::function<int(Ballot)> getBallotCandidateId = [](Ballot b) {
//    return b.topCandidate().id;
//  };
//  std::unordered_map<int, std::vector<Ballot>> ballots_by_candidate =
//      groupByKey(ballots, getBallotCandidateId);
template <class K, class T>
std::unordered_map<K, std::vector<T>> groupByKey(std::vector<T> elements,
                                                 std::function<K(T)> key) {
  std::unordered_map<K, std::vector<T>> result;
  for (const auto &el : elements) {
    K k = key(el);
    if (result.count(k)) {
      result.at(k).push_back(el);
    } else {
      result.emplace(k, std::vector<T>{el});
    }
  }
  return result;
}

// Struct describing the elimination of a candidate
struct EliminationInfo {
  // the candidate eliminated
  Candidate eliminatedCandidate;
  // the number of ballots exhausted (i.e. with no remaining choices)
  ulong exhaustedBallots{};
  // the number of ballots transferred to their next choice
  ulong transferredBallots{};
};

// A (sort of) double-ended priority queue to count the votes.
class ElectionQueue {
private:
  // map of candidates by their id; mostly a convenience
  std::unordered_map<int, Candidate> candidates_by_id;
  // a max heap of CandidateVotes (ordered by number of votes)
  std::vector<CandidateVotes> maxVoteHeap;
  // a min heap of CandidateVotes (ordered by number of votes)
  std::vector<CandidateVotes> minVoteHeap;
  // map of candidate vote totals by candidate id -- use this to transfer votes
  // and just maintain the heaps
  std::unordered_map<int, CandidateVotes> votes_by_candidate;
  // number of ballots still active
  ulong totalVotes{};

  int* find_next_candidate(Ballot&);

public:
  ElectionQueue() = default;
  ElectionQueue(const std::vector<Candidate> &, const std::vector<Ballot> &);
  // number of candidates still active
  int numCandidates();
  // eliminate the lowest candidate (and transfer their votes)
  // should throw an exception if there's a winner
  EliminationInfo eliminate();
  // check whether the conetst has a winner -- either one candidate remains
  // or one candidate has more than half the votes
  bool hasWinner();
  // get the winner -- throws a runtime error if there is no winner
  Candidate &winner();
  // helper function to max-heapify a vector of CandidateVotes
  static void max_heapify(std::vector<CandidateVotes> &);
  // helper function to min-heapify a vector of CandidateVotes
  static void min_heapify(std::vector<CandidateVotes> &);
};

} // namespace assignment5

#endif // ASSIGNMENT_5_ELECTION_QUEUE_HPP

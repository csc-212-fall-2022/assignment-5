#include "ElectionQueue.hpp"
#include "CandidateVotes.hpp"
#include <algorithm>
#include <functional>
#include <stdexcept>

namespace assignment5 {
ElectionQueue::ElectionQueue(const std::vector<Candidate> &candidates,
                             const std::vector<Ballot> &ballots) {
  // populate candidates_by_id
  for (const Candidate& candidate : candidates) {
    // insert into a map
    candidates_by_id.emplace(candidate.id, candidate);
    // candidates_by_id[candidate.id] = candidate should work (but we might need a default constructor for candidate)
  }

  std::function<int(Ballot)> getBallotCandidateId = [](Ballot b) {return b.topCandidate().id;};

  std::unordered_map<int, std::vector<Ballot>> ballots_by_candidate = groupByKey(ballots, getBallotCandidateId);

  std::vector<CandidateVotes> candVoteArray;
  for (const auto &[candidateId, candidateBallots] : ballots_by_candidate) {
    CandidateVotes candVotes = CandidateVotes{candidates_by_id.at(candidateId), candidateBallots};
    votes_by_candidate.emplace(candidateId, candVotes);
    candVoteArray.push_back(candVotes);
  }
  maxVoteHeap = candVoteArray;
  max_heapify(maxVoteHeap);

  totalVotes = ballots.size();
}

int ElectionQueue::numCandidates() { return candidates_by_id.size(); }

bool ElectionQueue::hasWinner() {
  if (candidates_by_id.size() == 1) {
    return true;
  }
  // check if someone (i.e. the leader) has half the active ballots (i.e. half the total votes)
  CandidateVotes leaderVotes = maxVoteHeap.front();
  return leaderVotes.votes() > totalVotes /2;
}

Candidate &ElectionQueue::winner() {
  // check hasWinner == true (else throw exception), return the top of the heap
  throw std::runtime_error("Implement me!"); }

EliminationInfo ElectionQueue::eliminate() {
    // pop from min heap -- a three step process
    // pop_heap moves the top element to the back of the vector and reheapifies the other elements
    std::pop_heap(minVoteHeap.begin(), minVoteHeap.end(), std::greater<>{});
    // get the last element
    CandidateVotes eliminated = minVoteHeap.back();
    // delete the last element (i.e. the eliminated candidate)
    minVoteHeap.pop_back();

    ulong ballotsExhausted = 0;
    ulong ballotsTransfered = 0;


    // remove the eliminated candidate from candidates_by_id

    // now the messy part -- redistribute the ballots from the eliminated candidate
    for (Ballot& ballot : eliminated.ballots()) {
      int *nextCandidateId = find_next_candidate(ballot);
      if (nextCandidateId != nullptr) {
        votes_by_candidate.at(*nextCandidateId).add_ballot(ballot);
        ballotsTransfered++;
      } else {
        ballotsExhausted++;
        totalVotes--;
      }// else ballot is exhausted
    }

    // repair the heaps -- reconstruct the heaps

    return EliminationInfo{candidates_by_id.at(eliminated.candidateId()), ballotsExhausted, ballotsTransfered};
}

void ElectionQueue::max_heapify(std::vector<CandidateVotes> &vec) {
  // our default heap is a max heap, constructed with <
  std::make_heap(vec.begin(), vec.end());
}

void ElectionQueue::min_heapify(std::vector<CandidateVotes> &vec) {
  std::make_heap(vec.begin(), vec.end(), std::greater<>{});
}

int *ElectionQueue::find_next_candidate(Ballot &ballot) {
  ballot.preferences.pop_front(); // get rid of the candidate that we just eliminated

  // find the first element of ballot.preferences that is still an active candidate
  while (candidates_by_id.count(ballot.topCandidate().id) == 0) {
    ballot.preferences.pop_front();
  }
  // now whoever is at the front is active
  if (ballot.isExhausted()) {
    return nullptr;
  } else {
    return &ballot.topCandidate().id;
  }
}

} // namespace assignment5
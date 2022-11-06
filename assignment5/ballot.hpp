#ifndef ASSIGNMENT_5_BALLOT_HPP
#define ASSIGNMENT_5_BALLOT_HPP

#include <deque>
#include <filesystem>
#include <optional>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace fs = std::filesystem;

namespace assignment5 {
struct Candidate {
  std::string name;
  int id;
  int contestId;

  std::ostream &operator<<(std::ostream &os) const {
    os << "Candidate{" << name << "," << id << "," << contestId << "}";
    return os;
  }

  bool operator==(const Candidate &rhs) const {
    return name == rhs.name && id == rhs.id && contestId == rhs.contestId;
  }

  bool operator<(const Candidate &rhs) const {
    // this should not happen unless they're equal, but just in case
    if (name == rhs.name) {
      return id < rhs.id;
    }
    return name < rhs.name;
  }

  bool operator>(const Candidate &rhs) const {
    if (name == rhs.name) {
      return id > rhs.id;
    }
    return name > rhs.name;
  }
};

struct Contest {
  std::string name;
  int id;
  int district;

  bool operator==(const Contest &rhs) const {
    return name == rhs.name && id == rhs.id;
  }
};

struct Preference {
  int voterId;
  int candidateId;
  int contestId;
  int rank;
};

struct Ballot {
  int voterId;
  int contestId;
  std::deque<Candidate> preferences;

  bool operator==(const Ballot &rhs) const {
    return voterId == rhs.voterId && contestId == rhs.contestId &&
           preferences == rhs.preferences;
  }

  bool isExhausted() const { return preferences.empty(); }

  Candidate &topCandidate() {
    return const_cast<Candidate &>(preferences.at(0));
  }
};

struct ElectionData {
  std::vector<Contest> contests;
  std::unordered_map<int, std::vector<Candidate>> candidates;
  std::unordered_map<int, std::vector<Ballot>> ballots;
};

ElectionData LoadElectionData(fs::path ballotImage, fs::path masterLookup);

std::optional<std::variant<Candidate, Contest>>
ParseElectionMasterLine(const std::string &);
} // namespace assignment5

#endif // ASSIGNMENT_5_BALLOT_HPP
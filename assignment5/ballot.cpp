#include "ballot.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/format.hpp"
#include "unicode/unistr.h"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <optional>
#include <regex>
#include <stdexcept>
#include <string>
#include <tuple>
#include <unordered_map>
#include <variant>
#include <vector>

namespace assignment5 {

const std::regex DISTRICT_REGEX = std::regex("Board of Supervisors, District (\\d+)"); // NOLINT

bool isZero(char c) { return c == '0'; }

bool preference_comp(const Preference &lhs, const Preference &rhs) {
  return lhs.rank < rhs.rank;
}

Ballot ConstructBallot(const std::unordered_map<int, Candidate> &candidates,
                       std::vector<Preference> &preferences) {
  // first, sort the preferences in ascending order by rank
  std::sort(preferences.begin(), preferences.end(), &preference_comp);

  int contestId = -1;
  // next, find the candidates associated to the preferences
  std::deque<Candidate> rankedCandidates;
  for (auto preference : preferences) {
    const Candidate &cand = candidates.at(preference.candidateId);
    if (contestId > 0 && preference.contestId != contestId) {
      throw std::runtime_error("Mismatched contest id!");
    } else {
      contestId = preference.contestId;
    }
    rankedCandidates.push_back(cand);
  }

  return Ballot{preferences.at(0).voterId, contestId, rankedCandidates};
}

std::vector<Ballot> LoadBallots(const fs::path &ballotImage,
                                const std::vector<Candidate> &candidates) {
  std::unordered_map<int, Candidate> candidateMap;
  std::unordered_map<int, std::vector<Preference>> voterPreferences;

  for (const Candidate &candidate : candidates) {
    candidateMap.emplace(candidate.id, candidate);
  }

  std::vector<Ballot> ballots;

  std::fstream ifs(ballotImage);

  std::string line;
  while (std::getline(ifs, line)) {
    std::string contest = line.substr(0, 7);
    boost::trim_left_if(contest, isZero);
    std::string voterId = line.substr(7, 9);
    boost::trim_left_if(voterId, isZero);
    std::string rank = line.substr(33, 3);
    boost::trim_left_if(rank, isZero);
    std::string candidate = line.substr(36, 7);
    boost::trim_left_if(candidate, isZero);
    char overVote = line.at(43);
    char underVote = line.at(44);
    // if we have either an overvote or an undervote, we skip the line
    if (overVote == '1' || underVote == '1') {
      continue;
    }
    int voter = std::stoi(voterId);
    int candidateId = std::stoi(candidate);
    int contestId = std::stoi(contest);
    int r = std::stoi(rank);
    Preference pref{voter, candidateId, contestId, r};
    // need to check if this voter is already in our map and, if so, push
    if (voterPreferences.count(voter)) {
      voterPreferences.at(voter).push_back(pref);
    } else {
      voterPreferences.emplace(voter, std::vector<Preference>{pref});
    }
  }

  for (auto &voterPreference : voterPreferences) {
    Ballot ballot = ConstructBallot(candidateMap, voterPreference.second);
    ballots.push_back(ballot);
  }

  return ballots;
}

std::tuple<std::vector<Candidate>, std::vector<Contest>>
ParseMaster(const fs::path &masterLookup) {
  std::vector<Candidate> candidates;
  std::vector<Contest> contests;

  std::fstream ifs(masterLookup);

  std::string line;
  while (std::getline(ifs, line)) {
    std::optional<std::variant<Candidate, Contest>> parsed =
        ParseElectionMasterLine(line);
    if (parsed) {
      switch (parsed->index()) {
      // candidate
      case 0:
        candidates.push_back(std::get<Candidate>(parsed.value()));
        break;
      // contest
      case 1:
        contests.push_back(std::get<Contest>(parsed.value()));
        break;
      }
    }
  }
  return std::make_tuple(candidates, contests);
}

ElectionData LoadElectionData(fs::path ballotImage, fs::path masterLookup) {

  if (!fs::exists(ballotImage)) {
    throw std::runtime_error(
        (boost::format("Path %s to ballot image not found!") % ballotImage)
            .str());
  }
  if (!fs::exists(masterLookup)) {
    throw std::runtime_error(
        (boost::format("Path %s to master lookup file not fount!") %
         masterLookup)
            .str());
  }
  auto masterData = ParseMaster(masterLookup);
  auto ballots = LoadBallots(ballotImage, std::get<0>(masterData));

  std::unordered_map<int, std::vector<Ballot>> ballots_by_contest;
  for (const Ballot &ballot : ballots) {
    if (ballots_by_contest.count(ballot.contestId)) {
      ballots_by_contest.at(ballot.contestId).push_back(ballot);
    } else {
      ballots_by_contest.emplace(ballot.contestId, std::vector<Ballot>{ballot});
    }
  }

  std::unordered_map<int, std::vector<Candidate>> candidates_by_contest;
  for (const Candidate &candidate : std::get<0>(masterData)) {
    if (candidates_by_contest.count(candidate.contestId)) {
      candidates_by_contest.at(candidate.contestId).push_back(candidate);
    } else {
      candidates_by_contest.emplace(candidate.contestId,
                                    std::vector<Candidate>{candidate});
    }
  }

  return ElectionData{std::get<1>(masterData), candidates_by_contest,
                      ballots_by_contest};
}

std::optional<std::variant<Candidate, Contest>>
ParseElectionMasterLine(const std::string &line) {
  std::string type = (line.substr(0, 10));
  boost::trim_right(type);
  std::string sId = line.substr(10, 7);
  boost::trim_left_if(sId, isZero);
  std::string name = line.substr(17, 50);
  boost::trim_right(name);
  if (type == "Candidate") {
    // substr goes weird when we have non-ascii characters,
    // so we need to convert to a unicode string
    icu::UnicodeString ucandConId;
    icu::UnicodeString::fromUTF8(line).extract(74, 7, ucandConId);
    std::string candConId;
    ucandConId.toUTF8String(candConId);
    boost::trim_left_if(candConId, isZero);
    Candidate candidate = {name, std::stoi(sId), std::stoi(candConId)};
    return candidate;
  } else if (type == "Contest") {
    std::smatch match;
    if (!std::regex_search(name, match, DISTRICT_REGEX)) {
      throw std::runtime_error((boost::format("Failed to match %s") % name).str());
    }
    Contest contest = {name, std::stoi(sId), std::stoi(match[1])};
    return contest;
  }
  return std::nullopt;
}
} // namespace assignment5
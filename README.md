# Assignment 5

In this assignment, we'll use a priority queue to simulate the tabulation of the votes from the 2016 San Francisco Board of Supervisors election. (Why 2016 and why the SF Board of Supervisors, you ask? They have ballot-level data available and the dataset is a reasonable size.)

This assignment is a bit different from previous assignments, as it's more focused on building a program to accomplish a task. Rather than implementing the data structure ourselves, we'll use the C++ standard library more than we have been.

> **Warning!**
> This assignment uses the [Boost](https://www.boost.org/) libraries. If you have `boost` installed, `cmake` will find it. If not, it will download `boost` from Github and will take an obnoxiously long time to configure. If you go that route, try to avoid deleting your `build/` folder.
> - If you use the devcontainer, the `Dockerfile` for this assignment installs `boost`.
> - On the Mac, you can install with `brew install boost`.
> - On Ubuntu/Debian/etc, you can install with `sudo apt-get libboost-all-dev`. (On other Linuxes, I assume you know what you're doing.)

### Instructions

#### Step One: Download the data
Run `scripts/download_data.sh`. It will create a directory called `data` at the top level of the assignment and download two files:
- the ballot image, which is a record of every ballot cast in the election(!)
- the master lookup, which lets us decode the ballot image

#### Step Two: Learn about Ranked Choice Voting

NYC uses Ranked Choice Voting in city elections and has a website [explaining how it works](https://www.nyc.gov/site/civicengagement/voting/ranked-choice-voting.page).

Each voter ranks the candidates. (I haven't double checked, but I believe you could rank up to three candidates in this election.) 
The first choice votes are counted. If one candidate has more than half the votes, they are declared the winner. If not,
the candidate with the fewest votes is eliminated. Their votes are then transferred to the second choice candidate on those ballots,
and we repeat the process.

In subsequent rounds, it's possible that the "next" choice candidate has already been eliminated. The vote transfers to the top choice that remains in "active".

*Note*: I don't think ties occur in this data set, but I have arbitrarily chosen to break ties by the candidate's name. We can safely assume that's not what the law actually says in San Francisco.

#### Step Three: Write the code

Your job is to implement `ElectionQueue` class. You're free to change any of the private variables, I've just left the ones that I found useful. You should leave the public methods as they are, though, as they're used in `Election` and `main`.

You should feel free to write some test of your own if that's helpful. (Weirdly, `Election.test.cpp` has a test for `ElectionQueue` in it already.)

#### Step Four: Check Your Results

In your `build/` directory, you can run `./Election --data-dir ../data` to run the whole thing. If you're using VSCode, I created a 'lauch configuration' that should show up on the "Launch and Debug" pane.

The actual results of the election are [here](https://sfelections.sfgov.org/november-8-2016-election-results-detailed-reports).

> **Warning**
> I'm somewhat concerned I messed up the parsing of District 9. Let me know if you're succeding on the first few districts and
> then hitting an error.

### References
- The idea from this homework came from https://www.cs.jhu.edu/~jason/226/hw8/. 
- [std::unordered_map](https://cplusplus.com/reference/unordered_map/unordered_map/)
- [std::deque](https://cplusplus.com/reference/deque/deque/) (used for the list of preferences on a `Ballot`, so you can pop from the front when the candidate is eliminated)
- [std::make_heap](https://cplusplus.com/reference/algorithm/make_heap/) (with friends `pop_heap` and `push_heap`)
- [std::priority_queue](https://cplusplus.com/reference/queue/priority_queue/) (I don't think it's particularly useful here, but just so you know it exists)
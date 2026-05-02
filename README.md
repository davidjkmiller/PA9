(README generated with AI)

# Texas Hold'em (C++ & SFML 3)

Developed by:

Caroline Fischer - Section 10L

David Miller - Section 10L

James (Owen) Tweedt - Section 02L

Benjamin Siev - Section 02L

### Video link: https://youtu.be/cox-3bhto3M

A full-featured Texas Hold'em Poker game built in C++ using the SFML 3 library for rendering, window management, and audio. The game features a graphical user interface, intelligent hand-scoring logic, single-player vs. CPU gameplay, and an integrated test suite.

## 🃏 How to Play Texas Hold'em

Texas Hold'em is a community card poker game where players try to make the best possible five-card hand.

__The Objective__

Win chips by either having the highest-ranking hand at the final showdown or by convincing all other players to fold their hands before the showdown.

__The Setup__

1. The Blinds: Before cards are dealt, two players post mandatory bets called the "Small Blind" and "Big Blind" to start the pot.

2. The Deal: Every player is dealt two private cards (hole cards) face down.

3. The Board: Over the course of the round, five community cards are dealt face-up in the center of the table. Players use any combination of their two hole cards and the five community cards to form their best hand.

__Betting Rounds__

During each betting round, players can choose to:

- Call: Match the current highest bet.

- Raise: Increase the current bet.

- Fold: Surrender your cards and forfeit the pot.

__There are four stages of betting:__

1. Pre-Flop: Occurs after hole cards are dealt but before any community cards are revealed.

2. The Flop: Three community cards are dealt face-up.

3. The Turn: A fourth community card is dealt.

4. The River: The fifth and final community card is dealt.

5. Showdown: If two or more players remain after the final betting round, players reveal their cards. The highest hand wins the pot.

__Hand Rankings (Best to Worst)__

1. Royal Flush: A, K, Q, J, 10, all of the same suit.

2. Straight Flush: Five sequential cards of the same suit.

3. Four of a Kind: Four cards of the same rank.

4. Full House: Three of a kind plus a pair.

5. Flush: Any five cards of the same suit.

6. Straight: Five sequential cards of mixed suits.

7. Three of a Kind: Three cards of the same rank.

8. Two Pair: Two different pairs.

9. One Pair: Two cards of the same rank.

10. High Card: Highest single card when no other hand is made.

## 🛠️ Building the Project (For Developers)

This project uses vcpkg (in manifest mode) to manage dependencies and statically links to SFML. Follow these steps to build the project locally in Visual Studio.

__Prerequisites__

Visual Studio 2022 (with C++ Desktop Development workload)

Git

vcpkg installed on your system

__Step 1: Clone the Repository__

```
git clone <[your-repository-url](https://github.com/davidjkmiller/PA9)>
cd <your-repository-folder>
```

__Step 2: Integrate vcpkg__

If you haven't already integrated vcpkg with your Visual Studio environment, open your terminal (Developer Command Prompt or PowerShell), navigate to your global vcpkg installation folder, and run:

``` .\vcpkg integrate install ```


__Step 3: Configure Visual Studio for Static Linking__

Because this project uses static SFML libraries, you must configure Visual Studio to use the correct vcpkg "triplet" before building. If you skip this step, you will encounter `LNK2001: unresolved external symbol` errors.

Open the `.sln` file in Visual Studio.

Right-click the project in the Solution Explorer and select __Properties__.

Set __Configuration__ to All Configurations and Platform to x64.

Go to the __vcpkg__ tab on the left menu.

Ensure Use Vcpkg Manifest is set to Yes.

Set Vcpkg Triplet to x64-windows-static.

Go to C/C++ -> Preprocessor -> Preprocessor Definitions and ensure SFML_STATIC is present in the list.

Step 4: Build the Project

In the top menu, go to __Build -> Clean Solution__.

Go to __Build -> Build Solution__.

_Note: The first build may take several minutes as vcpkg automatically downloads the SFML source code and compiles the static libraries based on the vcpkg.json manifest. Subsequent builds will be faster._

## 📂 Asset Structure

Ensure you have the required assets in the working directory (where the executable runs). The project expects the following folder structure:

`Assets/menus/` (splashScreen.png, tableBG.png, etc.)

`Assets/cards/` (cardBack.png, standard 52-card face images)

`Assets/music/` (.mp3 files for background audio)

`Assets/CowboyMovie.ttf`

The project includes a suite of core logic tests (test_core.cpp) integrated directly into the startup sequence. Every time the application boots, it automatically verifies card generation, deck shuffling, and complex poker hand-scoring combinations before launching the graphical window.

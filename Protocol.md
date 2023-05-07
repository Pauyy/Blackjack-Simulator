# Protocol

## Overview

### Player
The number of players is determined at the start of the entire Game.  
Player 0 will always be the dealer, while Player 1 is the first player and so on.  

### Annotation
Each Card is represented as 2 `char`s or actually 1 `string` containing 2 `char`s  
Card Values
|Card|Representation|
|---|---|
|2|2|
|3|3|
|4|4|
|5|5|
|6|6|
|7|7|
|8|8|
|9|9|
|10|T|
|Jack|J|
|Queen|Q|
|King|K|
|Ace|A|

Suit
|Suit|Representation|
|---|---|
|Club|c|
|Spade|s|
|Heart|h|
|Diamond|d|

### Actions
There are 2 Actions that can be performed  
|Corresponding Number|Action|
|---|---|
|1|Stay|
|2|Hit|

#### Hit
Prompts that the player will take another card

#### Stand
Prompts that the player will stop taking any new cards

### Results
After the dealer finished dealing all cards for a hand every Player hand gets checked to see if they lost or won or drawn  
There are 5 Outcomes  
|Corresponding Number|Outcome|
|-|-|
|0|blackjack|
|1|win|
|2|draw|
|3|loose|
|4|bust|

## Procedure

The Protocol reselmbles around 8 Stages
1. Setup
2. Bets
3. First 2 Cards
4. Additional Cards
5. Dealer Cards
6. Showdown
7. Overview
8. Loop


# 1. Setup
In the Setup the number of Players will be determinded aswell as the starting amount of chips of evrybody
Output: `Number of Players|` expected input `int`  
Output: `Starting amount of Chips|` expected input `int`  

# 2. Bets
After the Setup every Player needs to bet
Output: `bet|pn|` where `n` is the number of the Player. (See [Player](#Player) for more information) expected input: amount of chips to bet as `int`  
If the player does not want to bet on the round the correct input would be `0`  

# 3. First 2 Cards
After all bets are in, the first two cards get dealt.  
Output: `deal|pn|sv` where `n` is the number of the Player, `s` ist the suit and `v` is the value. (See [annotation](#Annotation) for more information) 

# 4. Additional Cards
After the first 2 Cards are dealt each Player has the chance to take more cards or stop playing
Output: `action|pn|` where `n` is the number of the Player. expected input `int` resembling stand or hit (See [actions](#actions) for more information)  

# 5. Dealer Cards
After every Player decided to stop taking new cards or busted the dealer, deals his cards
Output: `action|p0|1` when his handValue is >= 17  
Output: `action|p0|2` when his handValue is < 17  
Output: `deal|p0|sv` where `s` is the suit and `v` is the value.  

# 6. Showdown
After everyone got his cards it is shown who won, lost or drawn  
Output: `result|pn|` where `n` is the number of the Player. (See [results](#Results) for more information)  

# 7. Overview
Output: `overview|pn|chips|123` where `n` is the number of the Player and `123` is the number of their chips  

# 8. Loop
After one Round was played the game can go on  
Output: `remainingCards|208` where 208 is the number of remaining cards  
Output: `shuffleCards|` expected input: `0` for no shuffeling or anything else for shuffeling
Output: `nextround|` expected input: `0` for stopping the game anythin else for playing another round  


## Example Games
```
Number of Players|3
Starting amount of Chips|100
bet|p1|20
bet|p2|100
bet|p3|5
deal|p0|hJ
deal|p1|h3
deal|p2|d4
deal|p3|h2
deal|p0|sJ
deal|p1|c5
deal|p2|c7
deal|p3|c7
action|p1|2
deal|p1|c3
action|p2|2
deal|p2|cJ
action|p3|2
deal|p3|c6
action|p1|2
deal|p1|c4
action|p2|1
action|p3|2
deal|p3|dK
action|p1|2
deal|p1|cT
action|p0|1
result|p1|4
result|p2|1
result|p3|4
overview|p1|chips|80
overview|p2|chips|200
overview|p3|chips|95
remainingCards|194
nextround|0
```

```
Number of Players|2
Starting amount of Chips|100
bet|p1|20
bet|p2|30
deal|p0|h5
deal|p1|dQ
deal|p2|sJ
deal|p0|c2
deal|p1|c9
deal|p2|hA
action|p1|1
action|p0|2
deal|p0|d6
action|p0|2
deal|p0|dK
result|p1|1
result|p2|0
overview|p1|chips|120
overview|p2|chips|145
remainingCards|200
nextroud|0
```



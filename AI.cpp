#include "AI.h"

#include "cmath"
#include "queue"

#include "constants.h"

AI::AI() {
    start = clock();
    timeExceeded = false;
}

int AI::otherPlayer(int playerId) {
    if (playerId == PONE) {
        return PTWO;
    } else if (playerId == PTWO) {
        return PONE;
    }
    return playerId;
}

bool AI::timeout() {
    if (((float) ((clock() - start)) / CLOCKS_PER_SEC) > TIMELIMIT) {
        timeExceeded = true;
        return true;
    }
    return false;
}

int AI::mobility(Board* board, int playerId) {
    int countPlayer = 0, countOtherPlayer = 0;
    countPlayer = (int)board->getAvailableMoves(playerId).size();
    countOtherPlayer = (int)board->getAvailableMoves(otherPlayer(playerId)).size();
    return countPlayer - countOtherPlayer;
}

int AI::territory(Board* board, int playerId) {
    int i,j,k;
    int sign = pow(-1, playerId+1);

    int pOneQueenSquares = 0, pTwoQueenSquares = 0;
    int pOneQueenPly[SIZE][SIZE], pTwoQueenPly[SIZE][SIZE];

    int pOneKingSquares = 0, pTwoKingSquares = 0;
    int pOneKingPly[SIZE][SIZE], pTwoKingPly[SIZE][SIZE];

    int dirX[] = {-1, -1, -1, 0, 1, 1, 1, 0};
    int dirY[] = {-1, 0, 1, 1, 1, 0, -1, -1};

    for(i=0 ; i<SIZE ; i++) {
        for (j=0 ; j<SIZE ; j++) {
            pOneQueenPly[i][j] = INF;
            pTwoQueenPly[i][j] = INF;

            pOneKingPly[i][j] = INF;
            pTwoKingPly[i][j] = INF;
        }
    }

    // Storing minimum queen moves needed to reach each empty square
    for(i=0 ; i<SIZE ; i++) {
        for (j=0 ; j<SIZE ; j++) {
            if (board->getValue(i,j) == PONE) {
                // Modifying pOneQueenPly
                pOneQueenPly[i][j] = 0;
                std::queue<std::pair<int, int> > q;
                q.push(std::make_pair(i,j));
                while (!q.empty()) {
                    std::pair<int, int> front = q.front();
                    int a = front.first, b = front.second;
                    q.pop();
                    for (k=0 ; k<8 ; k++) {
                        int steps = 1;
                        while (a+steps*dirX[k] >= 0 && a+steps*dirX[k] < SIZE && b+steps*dirY[k] >= 0 && b+steps*dirY[k] < SIZE && board->getValue(a+steps*dirX[k], b+steps*dirY[k]) == EMPTY) {
                            if (pOneQueenPly[a][b] < pOneQueenPly[a+steps*dirX[k]][b+steps*dirY[k]]) {
                                pOneQueenPly[a+steps*dirX[k]][b+steps*dirY[k]] = pOneQueenPly[a][b] + 1;
                                q.push(std::make_pair(a+steps*dirX[k], b+steps*dirY[k]));
                            } else {
                                break;
                            }
                            steps++;
                        }
                    }
                }

                // Modifying pOneKingPly
                pOneKingPly[i][j] = 0;
                q.push(std::make_pair(i, j));
                while (!q.empty()) {
                    std::pair<int, int> front = q.front();
                    int a = front.first, b = front.second;
                    q.pop();
                    for (k=0 ; k<8 ; k++) {
                        if (a+dirX[k] >= 0 && a+dirX[k] < SIZE && b+dirY[k] >= 0 && b+dirY[k] < SIZE && board->getValue(a+dirX[k], b+dirY[k]) == EMPTY) {
                            if (pOneKingPly[a][b] + 1 < pOneKingPly[a+dirX[k]][b+dirY[k]]) {
                                pOneKingPly[a+dirX[k]][b+dirY[k]] = pOneKingPly[a][b] + 1;
                                q.push(std::make_pair(a+dirX[k], b+dirY[k]));
                            }
                        }
                    }
                }

            } else if (board->getValue(i, j) == PTWO) {
                // Modify pTwoQueenPly
                pTwoQueenPly[i][j] = 0;
                std::queue<std::pair<int, int> > q;
                q.push(std::make_pair(i,j));
                while (!q.empty()) {
                    std::pair<int, int> front = q.front();
                    int a = front.first, b = front.second;
                    q.pop();
                    for (k=0 ; k<8 ; k++) {
                        int steps = 1;
                        while (a+steps*dirX[k] >= 0 && a+steps*dirX[k] < SIZE && b+steps*dirY[k] >= 0 && b+steps*dirY[k] < SIZE && board->getValue(a+steps*dirX[k], b+steps*dirY[k]) == EMPTY) {
                            if (pTwoQueenPly[a][b] < pTwoQueenPly[a+steps*dirX[k]][b+steps*dirY[k]]) {
                                pTwoQueenPly[a+steps*dirX[k]][b+steps*dirY[k]] = pTwoQueenPly[a][b] + 1;
                                q.push(std::make_pair(a+steps*dirX[k], b+steps*dirY[k]));
                            } else {
                                break;
                            }
                            steps++;
                        }
                    }
                }

                //Modifying pTwoKingPly
                pTwoKingPly[i][j] = 0;
                q.push(std::make_pair(i, j));
                while (!q.empty()) {
                    std::pair<int, int> front = q.front();
                    int a = front.first, b = front.second;
                    q.pop();
                    for (k=0 ; k<8 ; k++) {
                        if (a+dirX[k] >= 0 && a+dirX[k] < SIZE && b+dirY[k] >= 0 && b+dirY[k] < SIZE && board->getValue(a+dirX[k], b+dirY[k]) == EMPTY) {
                            if (pTwoKingPly[a][b] + 1 < pTwoKingPly[a+dirX[k]][b+dirY[k]]) {
                                pTwoKingPly[a+dirX[k]][b+dirY[k]] = pTwoKingPly[a][b] + 1;
                                q.push(std::make_pair(a+dirX[k], b+dirY[k]));
                            }
                        }
                    }
                }
            }
        }
    }


    double w = 0.0;
    for(i=0 ; i<SIZE ; i++) {
        for (j=0 ; j<SIZE ; j++) {
            if (board->getValue(i,j) == EMPTY) {
                if (pOneQueenPly[i][j] < pTwoQueenPly[i][j]) {
                    pOneQueenSquares++;
                } else if (pTwoQueenPly[i][j] < pOneQueenPly[i][j]) {
                    pTwoQueenSquares++;
                }

                if (pOneKingPly[i][j] < pTwoKingPly[i][j]) {
                    pOneKingSquares++;
                } else if (pTwoKingPly[i][j] < pOneKingPly[i][j]) {
                    pTwoKingSquares++;
                }

                if (pOneQueenPly[i][j] != INF && pTwoQueenPly[i][j] != INF) {
                    w += pow(2, -std::abs(pOneQueenPly[i][j]-pTwoQueenPly[i][j]));
                }
            }
        }
    }

    int value = (int)((100-w)*(pOneQueenSquares - pTwoQueenSquares) + w*(pOneKingSquares - pTwoKingSquares));
    // int value = (int)((100)*(pOneQueenSquares - pTwoQueenSquares));
    if (playerId == PONE) {
        return value;
        // return pOneQueenSquares - pTwoQueenSquares;
    } else {
        return -value;
        // return pTwoQueenSquares - pOneQueenSquares;
    }

}

int AI::territoryAndMobility(Board* board, int playerId) {
    int i,j,k;
    int weight = 4;
    int pOnePoints = 0, pTwoPoints = 0;
    int pOneStonePly[SIZE][SIZE], pTwoStonePly[SIZE][SIZE];
    int pOneMob[SIZE][SIZE], pTwoMob[SIZE][SIZE];

    int dirX[] = {-1, -1, -1, 0, 1, 1, 1, 0};
    int dirY[] = {-1, 0, 1, 1, 1, 0, -1, -1};

    for (i=0 ; i<SIZE ; i++) {
        for (j=0 ; j<SIZE ; j++) {
            pOneStonePly[i][j] = INF;
            pTwoStonePly[i][j] = INF;
            pOneMob[i][j] = 0;
            pTwoMob[i][j] = 0;
        }
    }

    for (i=0 ; i<SIZE ; i++) {
        for (j=0 ; j<SIZE ; j++) {
            if (board->getValue(i, j) == PONE) {

                pOneStonePly[i][j] = 0;
                std::queue<std::pair<int, int> > q;
                q.push(std::make_pair(i,j));
                while (!q.empty()) {
                    std::pair<int, int> front = q.front();
                    int a = front.first, b = front.second;
                    q.pop();
                    for (k=0 ; k<8 ; k++) {
                        int steps = 1;
                        while (a+steps*dirX[k] >= 0 && a+steps*dirX[k] < SIZE && b+steps*dirY[k] >= 0 && b+steps*dirY[k] < SIZE && board->getValue(a+steps*dirX[k], b+steps*dirY[k]) == EMPTY) {
                            if (pOneStonePly[a][b] < pOneStonePly[a+steps*dirX[k]][b+steps*dirY[k]]) {
                                pOneStonePly[a+steps*dirX[k]][b+steps*dirY[k]] = pOneStonePly[a][b] + 1;
                                q.push(std::make_pair(a+steps*dirX[k], b+steps*dirY[k]));
                            } else {
                                break;
                            }
                            steps++;
                        }
                    }
                }

                for (k=0 ; k<8 ; k++) {
                    int steps = 1;
                    while (i+steps*dirX[k] >= 0 && i+steps*dirX[k] < SIZE && j+steps*dirY[k] >= 0 && j+steps*dirY[k] < SIZE && board->getValue(i+steps*dirX[k], j+steps*dirY[k]) == EMPTY) {
                        pOneMob[i+steps*dirX[k]][j+steps*dirY[k]]++;
                        steps++;
                    }
                }
            } else if (board->getValue(i, j) == PTWO) {

                pTwoStonePly[i][j] = 0;
                std::queue<std::pair<int, int> > q;
                q.push(std::make_pair(i,j));
                while (!q.empty()) {
                    std::pair<int, int> front = q.front();
                    int a = front.first, b = front.second;
                    q.pop();
                    for (k=0 ; k<8 ; k++) {
                        int steps = 1;
                        while (a+steps*dirX[k] >= 0 && a+steps*dirX[k] < SIZE && b+steps*dirY[k] >= 0 && b+steps*dirY[k] < SIZE && board->getValue(a+steps*dirX[k], b+steps*dirY[k]) == EMPTY) {
                            if (pTwoStonePly[a][b] < pTwoStonePly[a+steps*dirX[k]][b+steps*dirY[k]]) {
                                pTwoStonePly[a+steps*dirX[k]][b+steps*dirY[k]] = pTwoStonePly[a][b] + 1;
                                q.push(std::make_pair(a+steps*dirX[k], b+steps*dirY[k]));
                            } else {
                                break;
                            }
                            steps++;
                        }
                    }
                }

                for (k=0 ; k<8 ; k++) {
                    int steps = 1;
                    while (i+steps*dirX[k] >= 0 && i+steps*dirX[k] < SIZE && j+steps*dirY[k] >= 0 && j+steps*dirY[k] < SIZE && board->getValue(i+steps*dirX[k], j+steps*dirY[k]) == EMPTY) {
                        pTwoMob[i+steps*dirX[k]][j+steps*dirY[k]]++;
                        steps++;
                    }
                }
            }
        }
    }

    for (i=0 ; i<SIZE ; i++) {
        for (j=0 ; j<SIZE ; j++) {
            if (board->getValue(i, j) == EMPTY) {
                if (pOneStonePly[i][j] < pTwoStonePly[i][j]) {
                    pOnePoints += weight * pOneMob[i][j];
                } else if (pTwoStonePly[i][j] < pOneStonePly[i][j]) {
                    pTwoPoints += weight * pTwoMob[i][j];
                }
            }
        }
    }

    if (playerId == PONE) {
        return pOnePoints - pTwoPoints;
    } else {
        return pTwoPoints - pOnePoints;
    }

}

int AI::minMobility(Board* board, int playerId) {
    int i,j,k;
    int minOne = INF, minTwo = INF;

    int dirX[] = {-1, -1, -1, 0, 1, 1, 1, 0};
    int dirY[] = {-1, 0, 1, 1, 1, 0, -1, -1};

    for (i=0 ; i<SIZE ; i++) {
        for (j = 0; j < SIZE; j++) {
            if (board->getValue(i, j) == PONE) {
                int availableMove = 0;
                for (k=0 ; k<8 ; k++) {
                    int steps = 1;
                    while (i+steps*dirX[k] >= 0 && i+steps*dirX[k] < SIZE && j+steps*dirY[k] >= 0 && j+steps*dirY[k] < SIZE && board->getValue(i+steps*dirX[k], j+steps*dirY[k]) == EMPTY) {
                        availableMove++;
                        steps++;
                    }
                }
                if (availableMove != 0 && availableMove < minOne) { // Assuming that this condition will be true for at least one queen, since if availableMove is zero for all, game ends
                    minOne = availableMove;
                }
            } else if (board->getValue(i, j) == PTWO) {
                int availableMove = 0;
                for (k=0 ; k<8 ; k++) {
                    int steps = 1;
                    while (i+steps*dirX[k] >= 0 && i+steps*dirX[k] < SIZE && j+steps*dirY[k] >= 0 && j+steps*dirY[k] < SIZE && board->getValue(i+steps*dirX[k], j+steps*dirY[k]) == EMPTY) {
                        availableMove++;
                        steps++;
                    }
                }
                if (availableMove != 0 && availableMove < minTwo) {
                    minTwo = availableMove;
                }
            }
        }
    }

    int value = (int)(10000*(((float)(minOne - minTwo))/26));
    if (playerId == PONE) {
        return value;
    } else {
        return -value;
    }
}

int AI::regions(Board* board, int playerId) {
    int arr[SIZE][SIZE] = {0};
    int i, j, k;

    int dirX[] = {-1, -1, -1, 0, 1, 1, 1, 0};
    int dirY[] = {-1, 0, 1, 1, 1, 0, -1, -1};

    std::vector<std::pair<int, int> > pOnePieces, pTwoPieces;

    int region = 1;
    int count;
    std::vector<int> regionCount;
    regionCount.push_back(0);

    // Dividing board into regions and marking regions in a separate array
    for (i=0 ; i<SIZE ; i++) {
        for (j=0; j<SIZE ; j++) {
            if (board->getValue(i, j) == EMPTY && arr[i][j] == 0) {
                arr[i][j] = region;
                count = 1;
                std::queue<std::pair<int, int> > q;
                q.push(std::make_pair(i,j));
                while (!q.empty()) {
                    std::pair<int, int> front = q.front();
                    q.pop();
                    int x = front.first, y = front.second;
                    for (k=0 ; k<8 ; k++) {
                        if (x+dirX[k]>=0 && x+dirX[k]<SIZE && y+dirY[k]>=0 &&
                            y+dirY[k]<SIZE && board->getValue(x+dirX[k], y+dirY[k]) == EMPTY && arr[x+dirX[k]][y+dirY[k]] == 0) {
                            arr[x+dirX[k]][y+dirY[k]] = region;
                            count++;
                            q.push(std::make_pair(x+dirX[k], y+dirY[k]));
                        }

                    }
                }
                regionCount.push_back(count);
                region++;
            } else if (board->getValue(i, j) == PONE) {
                pOnePieces.push_back(std::make_pair(i,j));
            } else if (board->getValue(i, j) == PTWO) {
                pTwoPieces.push_back(std::make_pair(i, j));
            }
        }
    }

    // Two dimensional table in which value is true if piece in that region
    bool pOneRegionInfo[100][4] = {0}, pTwoRegionInfo[100][4] = {0};
    for (i=0 ; i<4 ; i++) {
        int x = pOnePieces[i].first, y = pOnePieces[i].second;
        for (k=0 ; k<8 ; k++) {
            if (x+dirX[k]>=0 && x+dirX[k]<SIZE && y+dirY[k]>=0 &&
                y+dirY[k]<SIZE && arr[x+dirX[k]][y+dirY[k]] != 0) {
                pOneRegionInfo[arr[x+dirX[k]][y+dirY[k]]][i] = true;
            }
        }

        x = pTwoPieces[i].first;
        y = pTwoPieces[i].second;
        for (k=0 ; k<8 ; k++) {
            if (x+dirX[k]>=0 && x+dirX[k]<SIZE && y+dirY[k]>=0 &&
                y+dirY[k]<SIZE && arr[x+dirX[k]][y+dirY[k]] != 0) {
                pTwoRegionInfo[arr[x+dirX[k]][y+dirY[k]]][i] = true;
            }
        }
    }

    // This calculates piece value for all pieces
    // By measuring dominant regions and shared regions of a piece on board
    int pOneValue[4], pTwoValue[4];
    for (i=0 ; i<4 ; i++) {
        int numDominant = 0, numShared = 0;
        for (j=1; j<region ; j++) {
            if (pOneRegionInfo[j][i]) {
                bool flag = false;
                for (k=0 ; k<4 ; k++) {
                    if (pTwoRegionInfo[j][k]) {
                        flag = true;
                        break;
                    }
                }
                if (!flag) {
                    numDominant++;
                } else {
                    numShared++;
                }
            }
        }
        pOneValue[i] = 2*numDominant + numShared;
    }

    for (i=0 ; i<4 ; i++) {
        int numDominant = 0, numShared = 0;
        for (j=1; j<region ; j++) {
            if (pTwoRegionInfo[j][i]) {
                bool flag = false;
                for (k=0 ; k<4 ; k++) {
                    if (pOneRegionInfo[j][k]) {
                        flag = true;
                        break;
                    }
                }
                if (!flag) {
                    numDominant++;
                } else {
                    numShared++;
                }
            }
        }
        pTwoValue[i] = 2*numDominant + numShared;
    }

    // Calculating score for each player
    float pOneScore = 0.0;
    float pTwoScore = 0.0;
    for (i=1 ; i<region ; i++) {
        int score = regionCount[i];
        float pOneNum = 0.0, pTwoNum = 0.0;

        for (j=0 ; j<4 ; j++) {
            if (pOneRegionInfo[i][j]) {
                pOneNum += 1.0/pOneValue[j];
            }
            if (pTwoRegionInfo[i][j]) {
                pTwoNum += 1.0/pTwoValue[j];
            }
        }
        if (pOneNum == 0.0 && pTwoNum == 0.0) {
            // For closed regions which do not have either black or white
            // This caused zero by zero exception
            continue;
        }
        pOneScore += score*pOneNum/(pOneNum + pTwoNum);
        pTwoScore += score*pTwoNum/(pOneNum + pTwoNum);
    }

    // Calculating final return value
    int value = (int)((10000*(pOneScore - pTwoScore))/(pOneScore + pTwoScore));
    if (playerId == PONE) {
        return value;
    } else {
        return -value;
    }

}

int AI::eval(Board* board, int playerId) {
    //return mobility(board, playerId);
    //return territory(board, playerId);
    //return territoryAndMobility(board, playerId);

    // Best eval
    // 2*regions + 5*territory + 3*minmobility
//    int region = regions(board, playerId);
//    int territor = territory(board, playerId);
//    int mobile = minMobility(board, playerId);
//    printf("%d %d %d\n", region, territor, mobile);
//    printf("%d %d %d\n", 2*region, 5*territor, 3*mobile);
//    return 2*region + 5*territor + 3*mobile;
    if (board->getTurnNumber() < 35) {
        return 3*regions(board, playerId) + 5*territory(board, playerId) + 2*minMobility(board, playerId);
    } else {
        return territory(board, playerId);
    }
    //return regions(board, playerId);
}

int AI::negaMax(Board board, int depth, int playerId) {

    if (depth == 0 || timeout()) {
        return eval(&board, otherPlayer(playerId));
    }

    int score = -INF, currentScore;
    int i;
    int movesSize;
    std::vector<Move> moves = board.getAvailableMoves(playerId);
    movesSize = (int)moves.size();
    for (i=0 ; i<movesSize ; i++) {
        board.playMove(moves[i]);
        currentScore = negaMax(board, depth-1, otherPlayer(playerId));
        if (currentScore > score) {
            score = currentScore;
        }
        board.undoMove(moves[i]);
        if (timeout()) {
            break;
        }
    }
    return -score;
}

int AI::alphabeta(Board board, int depth, int playerId, int alpha, int beta) {

    if (depth == 0 || timeout()) {
        return eval(&board, otherPlayer(playerId));
    }

    int score = -INF, currentScore;
    std::vector<Move> moves = board.getAvailableMoves(playerId);
    orderMoves(&board, moves);
    int movesSize = (int) moves.size();
    int i=0;

    for (i=0 ; i<movesSize ; i++) {
        board.playMove(moves[i]);
        currentScore = alphabeta(board, depth - 1, otherPlayer(playerId),  -beta, -alpha);
        if (currentScore > score) {
            score = currentScore;
            history[moves[i].srcX][moves[i].srcY][moves[i].dstX][moves[i].dstY][moves[i].arrowX][moves[i].arrowY] = score;
        }
        if (currentScore > alpha) {
            alpha = currentScore;
        }
        board.undoMove(moves[i]);
        if (alpha >= beta || timeout()) {
            break;
        }
    }
    return -score;
}

int AI::negaScout(Board board, int depth, int playerId, int alpha, int beta) {
    if (depth == 0 || timeout()) {
        return eval(&board, playerId);
    }

    int score = -INF, currentScore;
    int n = beta;

    std::vector<Move> moves = board.getAvailableMoves(playerId);
    orderMoves(&board, moves);
    int movesSize = (int) moves.size();
    int i=0;

    for (i=0 ; i<movesSize ; i++) {
        board.playMove(moves[i]);
        currentScore = negaScout(board, depth - 1, otherPlayer(playerId),  -n, -alpha);
        if (currentScore > score) {
            if (n == beta || depth <= 1) {
                score = currentScore;
            } else {
                score = negaScout(board, depth - 1, otherPlayer(playerId), -beta, -currentScore);
            }
        }
        if (currentScore > alpha) {
            alpha = currentScore;
        }
        board.undoMove(moves[i]);
        if (alpha >= beta || timeout()) {
            break;
        }
        n = alpha + 1;
    }
    return -score;

}

void AI::orderMoves(Board* board, std::vector<Move> & moves) {
    // return;
    int totalMoves = (int) moves.size();
    if (totalMoves == 0) {
        return;
    }
    int playerId = board->getValue(moves[0].srcX, moves[0].srcY);
    int evals[totalMoves];
    int i, j, key;
    Move move;

    for (i=0 ; i<totalMoves ; i++) {
        board->playMove(moves[i]);
//        evals[i] = eval(board, playerId);
        evals[i] = history[moves[i].srcX][moves[i].srcY][moves[i].dstX][moves[i].dstY][moves[i].arrowX][moves[i].arrowY];
        board->undoMove(moves[i]);
    }

    for (i = 1; i < totalMoves; i++)
    {
        key = evals[i];
        move = moves[i];
        j = i-1;
        while (j >= 0 && evals[j] < key) // Use less than sign
        {
            evals[j+1] = evals[j];
            moves[j+1] = moves[j];
            j = j-1;
        }
        evals[j+1] = key;
        moves[j+1] = move;
    }
}


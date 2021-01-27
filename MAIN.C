typedef enum 
{
	false,
	true
	
}bool;

typedef struct
{
	int x;
	int y;
}Position;
typedef enum 
{
	DIRECTION_UP=0,
	DIRECTION_LEFT=1,
	DIRECTION_DOWN=2,
	DIRECTION_RIGHT=3
	
}Direction;

typedef struct 
{
	int iWidth;
	int iHeigth;
	int iNumholes;	
	struct Position * pHolesLocation;
}Board;
typedef int PieceId;
typedef struct 
{
	PieceId iPieceId;
	Direction eMoveDir;
	bool bNutHasFallen;
}Move;


// maximum number of tiles covered by any place
#define MAX_PIECE_SIZE 3
typedef struct
{
	PieceId iPieceId;
	int iNumTiles;
	Position aTilesLocations[MAX_PIECE_SIZE];
	bool bHasHole;
	Position sHolePos;
}PieceDef;

typedef enum 
{
	Rotation_0=0,
	Rotation_90=1,
	Rotation_180=2,
	Rotation_270=3
	
}Rotation;

typedef struct 
{
	Position sPos;
	Rotation eRot;
	
}PiecePos;
//stack of moves
typedef struct
{
	Move *pMoves;
	int iNumMoves;	
}Game;
void board_play_move(Board* pBoard,Move* sMove)
{
	// dont forget to set pMove->bNutHasFallen		
}
void board_unplay_move(Board* pBoard,Move* sMove)
{
	// dont forget to set pMove->bNutHasFallen		
}
void game_push_move(Game* pGame,Game sMove,Board* pBoard)
{
	board_play_move(pBoard,&sMove);
	
	pGame->pMoves[pGame->iNumMoves]=sMove;
	pGame->iNumMoves++;	
}
void game_pop_move(Game* pGame,Board* pBoard)
{
	pGame->iNumMoves--;
	Move* pLastMove=&(pGame->pMoves[pGame->iNumMoves]);
	
	board_unplay_move(pBoard,pLastMove);
		
		
}

void scan(Game * pGame,Board * pBoard, Game** ppSolutions,int * piNumSolutions)
{
	if(game_is_solution()==true)
	{
	//copy this game to solutions array
	Game *pSolution = game_duplicate(pGame);
	ppSolutions[*piNumSolutions]= pSolution;
	(*piNumSolutions)++;	
	}
	else
	{ //explorer deeper
	int iPieceId;
	int iDir;
	for (iPieceId=0;iPieceId<4;iPieceId++)
		{
		for (iDir=0;iDir<4;iDir++)
			{
			Move sMove;
			sMove.iPieceId=iPieceId;
			sMove.eMoveDir=iDir;
			if (move_is_valid(sMove,pGame,pBoard))
				{
					game_push_move(pGame,sMove,pBoard);
					scan(pGame,pBoard,ppSolutions,piNumSolutions);
					game_pop_move(pGame,pBoard);
				}
			}
		}
	}
};



void find_solutions(Board* pBoard, PieceDef* pPieceDefs, PiecePos* pPiecePos,int iNumPieces, Game** pSolutions,int * piNumSolutions )
{
	Game* pWorkGame= game_create();
	scan(pWorkGame,pBoard,pSolutions,piNumSolutions);
	game_delete(pWorkGame);	
};


int main (int argc,char* argv[])
{
	
	
	
return 0;
}
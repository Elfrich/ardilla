#include <stdlib.h>
#include <stdio.h>
typedef enum 
{
	false,
	true
	
}bool;
// Array of pointers to structures
typedef struct
{
	int iNum_Elements;
	void *pElements;
}Array;
void array_init(Array* pArray)
{
	pArray->iNum_Elements = 0;
	pArray->pElements = NULL;
}

void array_empty(Array* pArray)
{
	if(pArray->iNum_Elements>0)
		{
			free(pArray->pElements);
			pArray->pElements=NULL;
			pArray->iNum_Elements=0;
		}
		
		
}
/* Description of the material used for a game (How many of rows and colums in the board, description of the pieces)*/
typedef struct
{	//number of rows and columsof the board
	int iNumRows;
	int iNumCols;	
	Array sPieceDefs; //Array of PieceDef
	Array sHolePos; //Array of Position	
}Material;
Material * material_create(int iNumRows, int iNumCols)
{
	Material* pMaterial = (Material *)malloc(sizeof(Material));
	pMaterial->iNumRows = iNumRows;
	pMaterial->iNumCols = iNumCols;
	array_init(&pMaterial->sPieceDefs);
	array_init(&pMaterial->sHolePos);
	return pMaterial;	
}
void material_delete(Material* pMaterial)
{
	// type of pMaterial : Material*
	// type of *pMaterial : Material
	// type (*pMaterial).sPieceDefs : Array
	// type of pMaterial->PieceDefs : Array
	// type of &(pMaterial->sPieceDefs): Array*
	array_empty(&(pMaterial->sPieceDefs));
	array_empty(&(pMaterial->sHolePos));
	free(pMaterial);
}
void material_print(Material* pMaterial)
{
	printf("board size: %d x %d", pMaterial->iNumRows,pMaterial->iNumCols);
	
}

typedef struct
{
	int x;
	int y;
}Position;
typedef enum 
{
	DIRECTION_UP   =0,
	DIRECTION_LEFT =1,
	DIRECTION_DOWN =2,
	DIRECTION_RIGHT=3
	
}Direction;
typedef struct
{
	bool bCellIsCovered;
	bool bCellHasNut;
}CellState;
// state of every cell in the board
typedef struct 
{
	Material* pMaterial; //reference to the material used
	CellState* pCellState;
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
	Rotation_0  =0,
	Rotation_90 =1,
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

/* a challenge made of initial position of pieces*/
typedef struct
{
	Material* pMaterial;
	Array sPieceStartConfig; // array of PiecePos
}Puzzle;


void board_play_move(Board* pBoard,Move* sMove)
{
	// dont forget to set pMove->bNutHasFallen		
}
void board_unplay_move(Board* pBoard,Move* sMove)
{
	// dont forget to set pMove->bNutHasFallen		
}
void game_push_move(Game* pGame,Move sMove,Board* pBoard)
{
	board_play_move(pBoard,&sMove);
	
	pGame->pMoves[pGame->iNumMoves]=sMove;
	pGame->iNumMoves++;	
}
void game_pop_move(Game* pGame,Board* pBoard)
{
	Move* pLastMove;
	pGame->iNumMoves--;


	pLastMove=&(pGame->pMoves[pGame->iNumMoves]);
	
	board_unplay_move(pBoard,pLastMove);
		
		
}

#ifdef AQUI
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

#endif 

int main (int argc,char* argv[])
{
	Material* pMaterial=material_create(4,4);
	material_print(pMaterial);
	material_delete(pMaterial);
	
return 0;
}
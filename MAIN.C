#include <stdlib.h>
#include <stdio.h>
typedef enum 
{
	false,
	true
	
}bool;
//#define DEBUG_MESSAGE(message) (printf((message));)
typedef int PieceId;
void error(char* pMessage)
{
	printf("ERROR: %s", pMessage);
	exit(1);
}	
// Array of pointers to structures
typedef struct
{
	int iNumElements;
	void **ppElements;
}Array;
void array_init(Array* pArray)
{
	pArray->iNumElements = 0;
	pArray->ppElements = NULL;
}

void array_empty(Array* pArray)
{
	if(pArray->iNumElements>0)
		{
			free(pArray->ppElements);
			pArray->ppElements=NULL;
			pArray->iNumElements=0;
		}
		
		
}
/* Adds an element to the array*/
void array_add(Array* pArray,void* pElement)
{
	int iElement;
	void** ppOldElements = pArray->ppElements;
	// allocate a bigger array of pointers
	pArray->ppElements = malloc(sizeof(void*) * (pArray->iNumElements + 1));
	// move de elements from the old elements to the new elements
		for(iElement=0; iElement< pArray->iNumElements; iElement++)
		{
			pArray->ppElements[iElement]=ppOldElements[iElement];
		}	
	// now that its contants have been copied, free the old array of elements
	if (ppOldElements =! NULL)
		{
			free(ppOldElements);
			ppOldElements = NULL;
		}
			
	//add a pointer to the new element at the end
	pArray->ppElements[pArray->iNumElements]=pElement;
	
	pArray->iNumElements++;
	
	
}


typedef struct
{
	int iX;
	int iY;
}Position;

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

PieceDef* piece_def_create(PieceId iPieceId)
{
	PieceDef* pPiece=(PieceDef*)malloc(sizeof(PieceDef));
	pPiece->iPieceId = iPieceId;
	pPiece->iNumTiles =0;
	pPiece->bHasHole=false;
	return pPiece;
}
void piece_def_delete(PieceDef* pPiece)
{   
	printf("deleting piece %d\n",pPiece->iPieceId);
	free(pPiece);
}

void piece_def_print(PieceDef* pPiece)
{       int iTileIndex=0;
	printf("piece %d : ", pPiece->iPieceId);

	for(iTileIndex=0; iTileIndex < pPiece->iNumTiles;iTileIndex++ )
		{
			printf("(%d, %d)",pPiece->aTilesLocations[iTileIndex].iX,pPiece->aTilesLocations[iTileIndex].iY);
		}
	
	if(pPiece->bHasHole)
		{
		printf("(hole at %d, %d)",pPiece->sHolePos.iX,pPiece->sHolePos.iY);
		}
		printf("\n");
}

void piece_def_add_tile(PieceDef* pPiece,int iTilePosX,int iTilePosY)
{
	if( pPiece->iNumTiles >= MAX_PIECE_SIZE)
	{	
	error("max number of the tiles excedeed");
	}	
	pPiece->aTilesLocations[pPiece->iNumTiles].iX=iTilePosX;
	pPiece->aTilesLocations[pPiece->iNumTiles].iY=iTilePosY;
	pPiece->iNumTiles++;
}

void piece_def_add_hole(PieceDef* pPiece,int iHolePosX,int iHolePosY)
{
	if( pPiece->bHasHole == true)
	{	
	error("Hole has already been set");
	}	
	pPiece->sHolePos.iX=iHolePosX;
	pPiece->sHolePos.iY=iHolePosY;
	pPiece->bHasHole = true;
	
}

void material_add_piece_def(Material* pMaterial, PieceDef* pPieceDef)
{       // type de pMaterial: Material*
		// type de *Material: Material
		// type de (*pMaterial).sPieceDefs: Array
		// type de pMaterial->sPieceDefs: Array
		// type de &(pMaterial->sPieceDefs): Array*
		// & should be read as "addres of"
		// * should bu read as "pointed by"
		// eg: "Material* p" should be read material pointed by pArray
	array_add(&(pMaterial->sPieceDefs), pPieceDef);
}
void material_delete(Material* pMaterial)
{
	int iPieceIndex=0;
	for(iPieceIndex = 0; iPieceIndex < pMaterial->sPieceDefs.iNumElements; iPieceIndex++)
	{
		PieceDef* pPiece = pMaterial->sPieceDefs.ppElements[iPieceIndex];
		piece_def_delete(pPiece);
		pMaterial->sPieceDefs.ppElements[iPieceIndex]=NULL;
	}
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
{   int iPieceIndex=0;
	printf("board size: %d x %d\n", pMaterial->iNumRows,pMaterial->iNumCols);
	for(iPieceIndex = 0; iPieceIndex < pMaterial->sPieceDefs.iNumElements; iPieceIndex++)
	{
		PieceDef* pPiece = pMaterial->sPieceDefs.ppElements[iPieceIndex];
		piece_def_print(pPiece);
	}

}


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

typedef struct
{
	PieceId iPieceId;
	Direction eMoveDir;
	bool bNutHasFallen;
}Move;


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
	PieceDef* p1 = piece_def_create(1);
	piece_def_add_tile(p1,0, 0);
	piece_def_add_tile(p1,0, 1);
	piece_def_add_tile(p1,1, 0);
	piece_def_add_hole(p1,0,1);
	
	material_add_piece_def(pMaterial,p1);
	
	material_print(pMaterial);
	material_delete(pMaterial);
	

return 0;
}
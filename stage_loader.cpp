//=====================================================================
// 
// ステージローダー [stage_loader.cpp]
// Author : Kaito Iwasaki
//
//=====================================================================
#include "stage_loader.h"
#include "util.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************


//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************


//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************
void _Read_STAGESET(FILE *pFile, STAGE *pStage);
void _Read_WAVESET(FILE* pFile, int nWave, TIMELINE** dpTimeline);
void _Read_ENEMYSET(FILE* pFile, int nWave, TIMELINE** dpTimeline);
void _Read_MUSICSET(FILE* pFile, int nWave, TIMELINE** dpTimeline);
void _Read_BGSET(FILE* pFile, int nWave, TIMELINE** dpTimeline);

//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
char g_aStrFile[MAX_PATH] = {};


//=====================================================================
// ***** ステージ情報読み込み処理 *****
//=====================================================================
void LoadStage(const char* aFileName, STAGE* pStage)
{
	FILE* pFile;
	TIMELINE* pTimeline = &pStage->timeline[0];
	int nWave = 0;

	pFile = fopen(aFileName, "r");

	if (pFile == NULL) return;	// ファイルを開けなかったら終了

	while (fscanf(pFile, "%s", &g_aStrFile[0]) != EOF)
	{
		if (strstr(&g_aStrFile[0], "STAGESET") != NULL)
		{// ステージ情報読み込み
			_Read_STAGESET(pFile, pStage);
		}
		else if (strstr(&g_aStrFile[0], "TIMELINESET") != NULL)
		{// タイムライン情報読み込み
			do
			{
				if (fscanf(pFile, "%s", &g_aStrFile[0]) == EOF)
				{
					break;
				}
				else if (strstr(&g_aStrFile[0], "WAVESET") != NULL)
				{
					_Read_WAVESET(pFile, nWave, &pTimeline);
					nWave++;
				}
			}while (strstr(&g_aStrFile[0], "END_TIMELINESET") == NULL);
		}
	}
	fclose(pFile);
}

void _Read_STAGESET(FILE* pFile, STAGE* pStage)
{
	do
	{
		if (fscanf(pFile, "%s", &g_aStrFile[0]) == EOF)
		{
			break;
		}
		else if (strncmp(g_aStrFile, "WAVE_MAX", 8) == 0)
		{
			fscanf(pFile, "%d", &pStage->nMaxWave);
		}
		else if (strncmp(g_aStrFile, "WAVE_START", 10) == 0)
		{
			fscanf(pFile, "%d", &pStage->nCurrentWave);
		}
		else if (strncmp(g_aStrFile, "WAVE_INTERVAL", 13) == 0)
		{
			fscanf(pFile, "%d", &pStage->nWaveInterval);
		}

	} while (strstr(&g_aStrFile[0], "END_STAGESET") == NULL);
}

void _Read_WAVESET(FILE* pFile, int nWave, TIMELINE** dpTimeline)
{
	do
	{
		if (fscanf(pFile, "%s", &g_aStrFile[0]) == EOF)
		{
			break;
		}
		else if (strncmp(&g_aStrFile[0], "ENEMYSET", 8) == 0)
		{
			_Read_ENEMYSET(pFile, nWave, dpTimeline);
		}
		else if (strncmp(&g_aStrFile[0], "MUSICSET", 8) == 0)
		{
			_Read_MUSICSET(pFile, nWave, dpTimeline);
		}
		else if (strncmp(&g_aStrFile[0], "BGSET", 5) == 0)
		{
			_Read_BGSET(pFile, nWave, dpTimeline);
		}
	} while (strstr(&g_aStrFile[0], "END_WAVESET") == NULL);
}

void _Read_ENEMYSET(FILE* pFile, int nWave, TIMELINE** dpTimeline)
{
	TIMELINE timelineTemp;

	memset(&timelineTemp, 0, sizeof(TIMELINE));
	timelineTemp.posOffset = D3DXVECTOR3(0.0f, -50.0f, 0.0f);
	timelineTemp.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	timelineTemp.nWave = nWave;

	do
	{
		if (fscanf(pFile, "%s", &g_aStrFile[0]) == EOF)
		{
			break;
		}
		else if (strncmp(g_aStrFile, "TYPE", 4) == 0)
		{
			fscanf(pFile, "%d", &timelineTemp.nType);
		}
		else if (strncmp(g_aStrFile, "NUM", 3) == 0)
		{
			fscanf(pFile, "%d", &timelineTemp.nNumEnemy);
		}
		else if (strncmp(g_aStrFile, "POS2", 4) == 0)
		{
			fscanf(pFile, "%f", &timelineTemp.pos.x);
			fscanf(pFile, "%f", &timelineTemp.pos.y);
		}
		else if (strncmp(g_aStrFile, "POSOFFSET2", 10) == 0)
		{
			fscanf(pFile, "%f", &timelineTemp.posOffset.x);
			fscanf(pFile, "%f", &timelineTemp.posOffset.y);
		}
		else if (strncmp(g_aStrFile, "MOVE2", 5) == 0)
		{
			fscanf(pFile, "%f", &timelineTemp.move.x);
			fscanf(pFile, "%f", &timelineTemp.move.y);
		}
		else if (strncmp(g_aStrFile, "TIMEOFFSET", 10) == 0)
		{
			fscanf(pFile, "%d", &timelineTemp.nCountTimeOffset);
		}
		else if (strncmp(g_aStrFile, "TIME", 4) == 0)
		{
			fscanf(pFile, "%d", &timelineTemp.nCountTime);
		}
		else if (strncmp(g_aStrFile, "LIFE", 4) == 0)
		{
			fscanf(pFile, "%d", &timelineTemp.nLife);
		}
		else if (strncmp(g_aStrFile, "INVERSED", 8) == 0)
		{
			fscanf(pFile, "%s", &g_aStrFile[0]);

			timelineTemp.bInversed = (strncmp(g_aStrFile, "TRUE", 4) == 0);
		}
	} while (strstr(&g_aStrFile[0], "END_ENEMYSET") == NULL);

	Clamp(&timelineTemp.nNumEnemy, 1, MAX_TIMELINE);

	for (int nCount = 0; nCount < timelineTemp.nNumEnemy; nCount++, (*dpTimeline)++)
	{
		(*dpTimeline)->bSet = true;
		(*dpTimeline)->eventType = EVENTTYPE_SETENEMY;
		(*dpTimeline)->nType = timelineTemp.nType;
		(*dpTimeline)->pos = timelineTemp.pos + timelineTemp.posOffset * nCount;
		(*dpTimeline)->move = timelineTemp.move;
		(*dpTimeline)->nCountTime = timelineTemp.nCountTime + (nCount * timelineTemp.nCountTimeOffset);
		(*dpTimeline)->nWave = timelineTemp.nWave;
		(*dpTimeline)->bInversed = timelineTemp.bInversed;
	}

	(*dpTimeline)++;
}

void _Read_MUSICSET(FILE* pFile, int nWave, TIMELINE** dpTimeline)
{
	TIMELINE timelineTemp;

	memset(&timelineTemp, 0, sizeof(TIMELINE));
	timelineTemp.nWave = nWave;

	do
	{
		if (fscanf(pFile, "%s", &g_aStrFile[0]) == EOF)
		{
			break;
		}
		else if (strncmp(g_aStrFile, "TYPE", 4) == 0)
		{
			fscanf(pFile, "%d", &timelineTemp.nType);
		}
	} while (strstr(&g_aStrFile[0], "END_MUSICSET") == NULL);

	Clamp(&timelineTemp.nNumEnemy, 1, MAX_TIMELINE);

	(*dpTimeline)->bSet = true;
	(*dpTimeline)->eventType = EVENTTYPE_SETMUSIC;
	(*dpTimeline)->nType = timelineTemp.nType;
	(*dpTimeline)->nWave = timelineTemp.nWave;

	(*dpTimeline)++;
}

void _Read_BGSET(FILE* pFile, int nWave, TIMELINE** dpTimeline)
{
	TIMELINE timelineTemp;

	memset(&timelineTemp, 0, sizeof(TIMELINE));;
	timelineTemp.nWave = nWave;

	do
	{
		if (fscanf(pFile, "%s", &g_aStrFile[0]) == EOF)
		{
			break;
		}
		else if (strncmp(g_aStrFile, "MOVE", 4) == 0)
		{
			fscanf(pFile, "%f", &timelineTemp.move.y);
		}
	} while (strstr(&g_aStrFile[0], "END_BGSET") == NULL);

	Clamp(&timelineTemp.nNumEnemy, 1, MAX_TIMELINE);

	(*dpTimeline)->bSet = true;
	(*dpTimeline)->eventType = EVENTTYPE_SETBGSPEED;
	(*dpTimeline)->move = timelineTemp.move;
	(*dpTimeline)->nWave = timelineTemp.nWave;

	(*dpTimeline)++;
}

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


//=====================================================================
// ***** ステージ情報読み込み処理 *****
//=====================================================================
void LoadStage(const char* aFileName, STAGE* pStage)
{
	FILE* pFile;
	char aStrFile[MAX_PATH];
	TIMELINE* pTimeline = &pStage->timeline[0];

	pFile = fopen(aFileName, "r");

	if (pFile == NULL) return;

	while (fscanf(pFile, "%s", &aStrFile[0]) != EOF)
	{
		if (strstr(&aStrFile[0], "STAGESET") != NULL)
		{
			do
			{
				if (fscanf(pFile, "%s", &aStrFile[0]) == EOF)
				{
					break;
				}
				else if (strncmp(aStrFile, "WAVE_MAX", 8) == 0)
				{
					fscanf(pFile, "%d", &pStage->nMaxWave);
				}
				else if (strncmp(aStrFile, "WAVE_START", 10) == 0)
				{
					fscanf(pFile, "%d", &pStage->nCurrentWave);
				}
				else if (strncmp(aStrFile, "WAVE_INTERVAL", 13) == 0)
				{
					fscanf(pFile, "%d", &pStage->nWaveInterval);
				}

			} while (strstr(&aStrFile[0], "END_STAGESET") == NULL);
		}
		else if (strstr(&aStrFile[0], "TIMELINESET") != NULL)
		{// タイムライン情報読み込み
			TIMELINE timelineTemp;

			memset(&timelineTemp, 0, sizeof(TIMELINE));
			timelineTemp.posOffset = D3DXVECTOR3(0.0f, -50.0f, 0.0f);

			do
			{
				if (fscanf(pFile, "%s", &aStrFile[0]) == EOF)
				{
					break;
				}
				else if (strncmp(aStrFile, "TYPE", 4) == 0)
				{
					fscanf(pFile, "%d", &timelineTemp.nType);
				}
				else if (strncmp(aStrFile, "NUM", 3) == 0)
				{
					fscanf(pFile, "%d", &timelineTemp.nNumEnemy);
				}
				else if (strncmp(aStrFile, "WAVE", 4) == 0)
				{
					fscanf(pFile, "%d", &timelineTemp.nWave);
				}
				else if (strncmp(aStrFile, "POS2", 4) == 0)
				{
					fscanf(pFile, "%f", &timelineTemp.pos.x);
					fscanf(pFile, "%f", &timelineTemp.pos.y);
				}
				else if (strncmp(aStrFile, "POSOFFSET2", 10) == 0)
				{
					fscanf(pFile, "%f", &timelineTemp.posOffset.x);
					fscanf(pFile, "%f", &timelineTemp.posOffset.y);
				}
				else if (strncmp(aStrFile, "MOVE3", 4) == 0)
				{
					fscanf(pFile, "%f", &timelineTemp.move.x);
					fscanf(pFile, "%f", &timelineTemp.move.y);
				}
				else if (strncmp(aStrFile, "TIMEOFFSET", 10) == 0)
				{
					fscanf(pFile, "%d", &timelineTemp.nCountTimeOffset);
				}
				else if (strncmp(aStrFile, "TIME", 4) == 0)
				{
					fscanf(pFile, "%d", &timelineTemp.nCountTime);
				}
				else if (strncmp(aStrFile, "LIFE", 4) == 0)
				{
					fscanf(pFile, "%d", &timelineTemp.nLife);
				}
				else if (strncmp(aStrFile, "INVERSED", 8) == 0)
				{
					fscanf(pFile, "%s", &aStrFile[0]);

					timelineTemp.bInversed = (strncmp(aStrFile, "TRUE", 4) == 0);
				}
			}while (strstr(&aStrFile[0], "END_TIMELINESET") == NULL);

			Clamp(&timelineTemp.nNumEnemy, 1, MAX_TIMELINE);

			for (int nCount = 0; nCount < timelineTemp.nNumEnemy; nCount++, pTimeline++)
			{
				pTimeline->bSet = true;
				pTimeline->nType = timelineTemp.nType;
				pTimeline->pos = timelineTemp.pos + timelineTemp.posOffset * nCount;
				pTimeline->nCountTime = timelineTemp.nCountTime + (nCount * timelineTemp.nCountTimeOffset);
				pTimeline->nWave = timelineTemp.nWave;
				pTimeline->bInversed = timelineTemp.bInversed;
			}

			pTimeline++;
		}
	}
	fclose(pFile);
}
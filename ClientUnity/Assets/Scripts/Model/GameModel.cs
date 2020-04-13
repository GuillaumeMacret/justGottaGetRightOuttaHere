using SimpleJSON;
using System.Collections;
using System.Collections.Generic;
using System.Threading;
using UnityEngine;
using UnityEngine.Tilemaps;

public class GameModel : MonoBehaviour
{
    public bool needsFullRedraw = false;
    public Tilemap blocksTileMap;

    private int[][] m_blocksLayer;

    private const string m_terrainTilesPath = "Tiles/TerrainTiles/terrain_atlas";

    private void Update()
    {
        if (needsFullRedraw)
        {
            RefreshTilemap();
        }
    }

    private void RefreshTilemap()
    {
        for (int i = 0; i < m_blocksLayer.Length; ++i)
        {
            for (int j = 0; j < m_blocksLayer[i].Length; ++j)
            {
                blocksTileMap.SetTile(new Vector3Int(i, -j, 1), TilesResourcesLoader.GetTileByNameAndId(m_terrainTilesPath, m_blocksLayer[i][j]));
            }
        }
    }
    
    public void LoadBlocks(int xSize, int ySize, JSONArray matrix)
    {
        m_blocksLayer = new int[xSize][];
        for (int i = 0; i < xSize; ++i)
        {
            m_blocksLayer[i] = new int[ySize];
            for (int j = 0; j < ySize; ++j)
            {
                m_blocksLayer[i][j] = matrix[i][j];
            }
        }
    }

    /// <summary>
    /// Loads the level according to a json message received
    /// This includes : blocks layer, object layer and players
    /// </summary>
    /// <param name="loadLevelObject"></param>
    public void LoadLevel(int xSize, int ySize, JSONArray blocks)
    {

        LoadBlocks(xSize,ySize,blocks);
        //TODO objects layer
        //TODO players
        needsFullRedraw = true;
    }
}

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
    public Player playerPrefab;

    /*Tiles layers*/
    private int[][] m_blocksLayer;

    private List<Player> m_players;
    //The vec with info to create new players (xpos,ypos,id)
    private List<Vector3> m_playerToInstantiate = new List<Vector3>();

    private const string m_terrainTilesPath = "Tiles/TerrainTiles/terrain_atlas";

    private void Update()
    {
        if (needsFullRedraw)
        {
            RefreshTilemap();
        }
        if(m_playerToInstantiate.Count > 0)
        {
            Player p = Instantiate(playerPrefab);
            p.transform.position = new Vector3(m_playerToInstantiate[0][0], -m_playerToInstantiate[0][1],0);
            p.id = (int) m_playerToInstantiate[0][2];
            m_players.Add(p);

            GameObject[] container = GameObject.FindGameObjectsWithTag("PlayersContainer");
            if (container.Length > 0)
            {
                p.transform.SetParent(container[0].transform);
            }

            m_playerToInstantiate.RemoveAt(0);
        }
        //TODO move player sprites
    }

    /// <summary>
    /// Changes the tilemap blocsTileMap to correspond with the id store in the int matrix
    /// </summary>
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
    
    /// <summary>
    /// Sets the given Json matrix into the classe's int matrix corresponding to the block layer
    /// </summary>
    /// <param name="xSize"></param>
    /// <param name="ySize"></param>
    /// <param name="matrix"></param>
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
    /// Creates new players and set their position
    /// </summary>
    public void LoadPlayers(int nbPlayers, JSONArray playersArray)
    {
        m_players = new List<Player>();
        for(int i = 0; i < nbPlayers;++i)
        {
            m_playerToInstantiate.Add(new Vector3(playersArray[i]["xPos"], playersArray[i]["yPos"], i));
        }
    }

    /// <summary>
    /// Loads the level according to a json message received
    /// This includes : blocks layer, object layer and players
    /// </summary>
    /// <param name="loadLevelObject"></param>
    public void LoadLevel(JSONArray blocks, JSONArray players)
    {
        if (blocks.Count > 0)
        {
            LoadBlocks(blocks.Count, blocks[0].Count, blocks);
        }
        if(players.Count > 0)
        {
            LoadPlayers(players.Count, players);
        }
        //TODO other layers
        needsFullRedraw = true;
    }

    public void MovePlayer(int id, int xPos, int yPos)
    {
        m_players[id].AddDestination(xPos, yPos);
    }

}

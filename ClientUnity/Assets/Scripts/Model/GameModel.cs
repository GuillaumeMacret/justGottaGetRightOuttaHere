using SimpleJSON;
using System.Collections;
using System.Collections.Generic;
using System.Threading;
using UnityEngine;
using UnityEngine.Tilemaps;
using UnityEditor.Animations;

public class GameModel : MonoBehaviour
{
    public bool needsFullRedraw = false;
    public bool needsObjectsRedraw = false;
    public Tilemap blocksTileMap;
    public Tilemap objectsTileMap;
    public Player playerPrefab;
    public Camera mainCamera;
    public PlayerAnimatorControllerFactory animFactory;
    public List<AudioClip> musics;


    private static float m_GameTimer;
    public static float GameTimer { get => m_GameTimer; }

    private static string m_LevelName = "No name";
    public static string LevelName { get => m_LevelName; }

    private AudioSource m_audioSource;
    /*Tiles layers*/
    private int[][] m_blocksLayer;
    private int[][] m_objectsLayer;

    private List<Player> m_players;
    //The vec with info to create new players (xpos,ypos,id)
    private List<Vector3> m_playerToInstantiate = new List<Vector3>();

    private const string m_terrainTilesPath = "Tiles/MergeTerrainTiles/Spritesheetmerge";


    private void Awake()
    {
        m_audioSource = GetComponent<AudioSource>();
        m_GameTimer = 0;
    }

	private void Start() 
	{
		LoadLevel(GameLobbyData.BlocksJson, GameLobbyData.PlayersJson, GameLobbyData.ObjectsJson, GameLobbyData.LevelName);
	}

	private void startRandomMusic()
    {
        m_audioSource.clip = musics[Random.Range(0, musics.Count)];
        m_audioSource.Play();
    }
    private void Update()
    {
        m_GameTimer += Time.deltaTime;
        if (needsFullRedraw)
        {
            blocksTileMap.ClearAllTiles();
            objectsTileMap.ClearAllTiles();
            GameObject[] container = GameObject.FindGameObjectsWithTag("PlayersContainer");
            foreach (GameObject g in GameObject.FindGameObjectsWithTag("Player"))
            {
                Destroy(g);
            }
            RefreshBlocksTilemap();
            RefreshObjectsTilemap();
            UpdateCameraSettings();
            startRandomMusic();
            m_GameTimer = 0;
            needsFullRedraw = false;
        }
        else if (needsObjectsRedraw)
        {
            RefreshObjectsTilemap();
        }

        if(m_playerToInstantiate.Count > 0)
        {
            Player p = Instantiate(playerPrefab);
            //Debug.Log("Instatiating new player : " + m_playerToInstantiate[0][0] + m_playerToInstantiate[0][1] + m_playerToInstantiate[0][2]);
            p.id = (int) m_playerToInstantiate[0][2];
            AnimatorController animController = animFactory.GetAnimatorController(p.id);
            p.SetAnimatorController(animController);
            p.transform.position = new Vector3(m_playerToInstantiate[0][0], -m_playerToInstantiate[0][1],0);
            m_players.Add(p);
            Debug.Log(p.transform.position);

            GameObject[] container = GameObject.FindGameObjectsWithTag("PlayersContainer");
            if (container.Length > 0)
            {
                p.transform.SetParent(container[0].transform);
            }

            m_playerToInstantiate.RemoveAt(0);
        }
    }

    /// <summary>
    /// Sets the main camera position and size according to the size of the blocks layer
    /// </summary>
    private void UpdateCameraSettings()
    {
        /* Dertemine wich dimension is reaching the border */
        int width = m_blocksLayer[0].Length;
        int height = m_blocksLayer.Length;
        //Debug.Log("Sizes : " + width + " " + height);
        float widthRatio = width / 5.0f;
        float heightRatio = height / 8.0f;
        //Debug.Log("Ratios : " + widthRatio + " " + heightRatio);
        if (widthRatio > heightRatio)
        {
            //Debug.Log("Width is limiting");
            mainCamera.orthographicSize = (widthRatio * 8) / 2;
        }
        else
        {
            //Debug.Log("Height is limiting");
            mainCamera.orthographicSize = height / 2;
        }

        mainCamera.transform.position = new Vector3(width / 2.0f, -height / 2.0f, mainCamera.transform.position.z);
    }

    /// <summary>
    /// Changes the tilemap blocksTileMap to correspond with the id store in the int matrix
    /// </summary>
    private void RefreshBlocksTilemap()
    {
        if (m_blocksLayer == null) return;
        for (int i = 0; i < m_blocksLayer.Length; ++i)
        {
            for (int j = 0; j < m_blocksLayer[i].Length; ++j)
            {
                if(m_blocksLayer[i][j] > 0)
                {
                    blocksTileMap.SetTile(new Vector3Int(j, -i - 1, 1), TilesResourcesLoader.GetTileByNameAndId(m_terrainTilesPath, m_blocksLayer[i][j] - 1));
                }                
            }
        }
    }

    /// <summary>
    /// Changes the tilemap blocksTileMap to correspond with the id store in the int matrix
    /// </summary>
    private void RefreshObjectsTilemap()
    {
        if (m_objectsLayer == null) return;
        for (int i = 0; i < m_objectsLayer.Length; ++i)
        {
            for (int j = 0; j < m_objectsLayer[i].Length; ++j)
            {
                if (m_objectsLayer[i][j] > 0)
                {
                    objectsTileMap.SetTile(new Vector3Int(j, -i - 1, 2), TilesResourcesLoader.GetTileByNameAndId(m_terrainTilesPath, m_objectsLayer[i][j] - 1));
                }
                else
                {
                    objectsTileMap.SetTile(new Vector3Int(j, -i - 1, 2), null);
                }
                    
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
    /// Sets the objects int matrix acccording to Json given in argument
    /// </summary>
    /// <param name="xSize"></param>
    /// <param name="ySize"></param>
    /// <param name="objects"></param>
    public void LoadObjects(int xSize, int ySize, JSONArray objects)
    {
        m_objectsLayer = new int[xSize][];
        for (int i = 0; i < xSize; ++i)
        {
            m_objectsLayer[i] = new int[ySize];
            for (int j = 0; j < ySize; ++j)
            {
                m_objectsLayer[i][j] = objects[i][j];
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
            m_playerToInstantiate.Add(new Vector3(playersArray[i]["xPos"], playersArray[i]["yPos"], playersArray[i]["Role"]));
        }
    }

    /// <summary>
    /// Loads the level according to a json message received
    /// This includes : blocks layer, object layer and players
    /// </summary>
    /// <param name="loadLevelObject"></param>
    public void LoadLevel(JSONArray blocks, JSONArray players, JSONArray objects, string levelName)
    {
        m_LevelName = levelName;
        if (blocks.Count > 0)
        {
            LoadBlocks(blocks.Count, blocks[0].Count, blocks);
        }
        if (players.Count > 0)
        {
            LoadPlayers(players.Count, players);
        }
        //FIXME If switching from a map with object to a map without, previous objects stay
        // A map without objects should not exists so this is not an critical fix
        if (objects.Count > 0)
        {
            LoadObjects(objects.Count, objects[0].Count, objects);
        }
        needsFullRedraw = true;
    }

    /// <summary>
    /// Adds the given coordinates to the player's (identified by id) destination list.
    /// Destination list is a FIFO and so, the player will move there after completing his other movements
    /// </summary>
    /// <param name="id"></param>
    /// <param name="xPos"></param>
    /// <param name="yPos"></param>
    public void MovePlayer(int id, int xPos, int yPos)
    {
        m_players[id].AddDestination(xPos, -yPos);
    }

    public void UpdateObjects(JSONArray changes)
    {
        for(int i = 0; i < changes.Count; ++i)
        {
            Debug.Log("Changing object at " + changes[i]["yPos"] + " " + changes[i]["xPos"] +" from " + m_objectsLayer[changes[i]["yPos"]][changes[i]["xPos"]] + " to " + changes[i]["value"]);
            m_objectsLayer[changes[i]["yPos"]][changes[i]["xPos"]] = changes[i]["value"];
            needsObjectsRedraw = true;
        }
    }

}

using SimpleJSON;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Tilemaps;
using UnityEngine.UI;

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
    public bool gameWon = false;
    public GameObject virtualDPad;
    public GameObject winMessageContainer;
    public Image actionButtonImage;
    public Image pingButtonImage;
    public GameObject pingPrefab;
    public Button returnButton;
    public GameObject loadingBox;

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

    private List<Vector2> m_PingToInstantiate = new List<Vector2>();

    private const string m_terrainTilesPath = "Tiles/MergeTerrainTiles/Spritesheetmerge";

    /*Parameters to locate where is the ping*/
    private float m_TileSize;
    //The gap between the left of the screen and the first tile
    private float m_XStartOffset;
    //The gap between the bottom of the screen and the first tile
    private float m_YStartOffset;

    private const int UP = 1, DOWN = 2, LEFT = 3, RIGHT = 4;

    private bool m_SceneWasLocked = true;
    private GameObject[] m_ObjectsToLock;
    private GameObject m_LoadingBox;
    public bool CanUnlockScene = false;

    /// <summary>
    /// Looks for objects with tag "LockedDuringLoad" in scene and disable them
    /// Looks also for an object with tag "LoadingMessage" to enable it
    /// The goal here is to prevent players from sending update message while other are still loading the scene
    /// </summary>
    public void LockSceneInputs()
    {
        m_SceneWasLocked = true;
        foreach(GameObject o in m_ObjectsToLock)
        {
            o.SetActive(false);
        }
        m_LoadingBox.SetActive(true);
        CanUnlockScene = false;
    }

    /// <summary>
    /// Does the opposite of LockSceneInputs()
    /// </summary>
    public void UnlockSceneInputs()
    {
        foreach (GameObject o in m_ObjectsToLock)
        {
            o.SetActive(true);
        }
        m_LoadingBox.SetActive(false);
        m_GameTimer = 0;
    }

    private void Awake()
    {
        m_ObjectsToLock = GameObject.FindGameObjectsWithTag("LockedDuringLoad");
        m_LoadingBox = GameObject.FindGameObjectWithTag("LoadingMessage");
        LockSceneInputs();
        m_audioSource = GetComponent<AudioSource>();
        m_GameTimer = 0;
    }

    private bool EverythingHasBeenInstantiated()
    {
        if(m_playerToInstantiate.Count != 0)
        {
            return false;
        }
        return true;
    }

    private void Start() 
	{
        LoadLevel(GameLobbyData.BlocksJson, GameLobbyData.PlayersJson, GameLobbyData.ObjectsJson, GameLobbyData.LevelName);
        
        if (m_SceneWasLocked && EverythingHasBeenInstantiated())
        {
            string messageToSend = MessageBuilders.BuildRdyMessage();
            TCPClient.SendMessage(messageToSend);
            m_SceneWasLocked = false;
        }
    }

    /// <summary>
    /// Plays a music at random picked from the musics array
    /// </summary>
	private void StartRandomMusic()
    {
        m_audioSource.clip = musics[Random.Range(0, musics.Count)];
        m_audioSource.Play();
    }
    private void Update()
    {
        if (CanUnlockScene)
        {
            UnlockSceneInputs();
            CanUnlockScene = false;
        }
        if (GameLobbyData.PlayerId != 0)
        {
            returnButton.interactable = false;
        }
        else
        {
            returnButton.interactable = true;
        }
        if (gameWon)
        {
            virtualDPad.SetActive(false);
            winMessageContainer.SetActive(true);
        }
        else
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
                StartRandomMusic();
                m_GameTimer = 0;
                needsFullRedraw = false;
            }
            else if (needsObjectsRedraw)
            {
                RefreshObjectsTilemap();
            }

            if (m_playerToInstantiate.Count > 0)
            {
                Player p = Instantiate(playerPrefab);
                //Debug.Log("Instatiating new player : " + m_playerToInstantiate[0][0] + m_playerToInstantiate[0][1] + m_playerToInstantiate[0][2]);
                p.id = (int)m_playerToInstantiate[0][2];
                RuntimeAnimatorController animController = animFactory.GetAnimatorController(p.id);
                p.SetAnimatorController(animController);
                p.transform.position = new Vector3(m_playerToInstantiate[0][0], -m_playerToInstantiate[0][1], 0);
                m_players.Add(p);
                //Debug.Log(p.transform.position);

                GameObject[] container = GameObject.FindGameObjectsWithTag("PlayersContainer");
                if (container.Length > 0)
                {
                    p.transform.SetParent(container[0].transform);
                }

                m_playerToInstantiate.RemoveAt(0);
            }

            if(m_PingToInstantiate.Count > 0)
            {
                GameObject ping = Instantiate(pingPrefab);
                ping.transform.position = new Vector3(m_PingToInstantiate[0].x, -m_PingToInstantiate[0].y, 0);
                Debug.Log("New ping at :" + ping.transform.position);
                m_PingToInstantiate.RemoveAt(0);
            }
        }
    }

    /// <summary>
    /// Sets the main camera position and size according to the size of the blocks layer
    /// </summary>
    private void UpdateCameraSettings()
    {
        //Debug.Log("Screen Dimension : " + Screen.width + " " + Screen.height);
        /* Dertemine wich dimension is reaching the border */
        int width = m_blocksLayer[0].Length;
        int height = m_blocksLayer.Length;
        //Debug.Log("Sizes : " + width + " " + height);
        float aspectRatio = (float)Screen.width / (float)Screen.height;
        //Debug.Log("Aspect ratio : " + aspectRatio);

        float widthRatio = width / aspectRatio;
        float heightRatio = height;
        //Debug.Log("Ratios : " + widthRatio + " " + heightRatio);
        if (widthRatio > heightRatio)
        {
            //Debug.Log("Width is limiting");
            mainCamera.orthographicSize = width / aspectRatio / 2;
            //Debug.Log("New size is "+ widthRatio);
            m_TileSize = Screen.width / width;
            m_XStartOffset = 0;
            m_YStartOffset = (Screen.height - (height * m_TileSize)) / 2;
        }
        else
        {
            //Debug.Log("Height is limiting");
            mainCamera.orthographicSize = height / 2;
            m_TileSize = Screen.height / height;
            m_YStartOffset = 0;
            m_XStartOffset = (Screen.width - (width* m_TileSize)) / 2;
        }
        //Debug.Log("New tile size is : " + m_TileSize);

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
        LockSceneInputs();
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
        gameWon = false;
        winMessageContainer.SetActive(false);
    }

    /// <summary>
    /// Adds the given coordinates to the player's (identified by id) destination list.
    /// Destination list is a FIFO and so, the player will move there after completing his other movements
    /// </summary>
    /// <param name="id"></param>
    /// <param name="xPos"></param>
    /// <param name="yPos"></param>
    public void MovePlayer(int id, int xPos, int yPos, string directionFacing)
    {
        int facing = 0;
        switch (directionFacing)
        {
            case "up":
                facing = UP;
                break;
            case "down":
                facing = DOWN;
                break;
            case "left":
                facing = LEFT;
                break;
            case "right":
                facing = RIGHT;
                break;
        }
        m_players[id].AddDestination(xPos, -yPos, facing);
    }

    /// <summary>
    /// Updates the object matrix according to the JSON array given in argument
    /// </summary>
    /// <param name="changes"></param>
    public void UpdateObjects(JSONArray changes)
    {
        if (changes == null) return;
        if (m_objectsLayer.Length <= 0) Debug.LogError("Trying to change an object but the matrix is empty");
        for(int i = 0; i < changes.Count; ++i)
        {
            Debug.Log("Changing object at " + changes[i]["yPos"] + " " + changes[i]["xPos"] +" from " + m_objectsLayer[changes[i]["yPos"]][changes[i]["xPos"]] + " to " + changes[i]["value"]);
            m_objectsLayer[changes[i]["yPos"]][changes[i]["xPos"]] = changes[i]["value"];
            needsObjectsRedraw = true;
        }
    }

    /// <summary>
    /// Give the x and y of the tile clicked on according to the x and y postion of the pixel given in parameter
    /// </summary>
    /// <param name="x"></param>
    /// <param name="y"></param>
    /// <returns></returns>
    public Vector2 PixelPosToTilePos(int x,int y)
    {
        int width = m_blocksLayer[0].Length;
        int height = m_blocksLayer.Length;
        float tileX, tileY;
        tileX = (x - m_XStartOffset) / m_TileSize;
        tileY = (y - m_YStartOffset) / m_TileSize;
        if (tileX < 0) tileX = 0;
        if (tileY < 0) tileY = 0;
        if (tileX >= width) tileX = width - 1;
        if (tileY >= height) tileY = height - 1;

        /*Inverse Y because our origin is TOPleft while this one is BOTleft*/
        tileY = -(tileY - height);

        return new Vector2(tileX, tileY);
    }

    /// <summary>
    /// Adds a ping wich will be instantiated in the next update loop
    /// </summary>
    /// <param name="x"></param>
    /// <param name="y"></param>
    public void CreatePing(int x, int y)
    {
        m_PingToInstantiate.Add(new Vector2(x, y));
    }

    /// <summary>
    /// Sets the user's player facing direction
    /// </summary>
    /// <param name="x"></param>
    /// <param name="y"></param>
    public void SetPlayerFacing(int x,int y)
    {
        m_players[GameLobbyData.PlayerId].SetFacing(x, y);
    }
}

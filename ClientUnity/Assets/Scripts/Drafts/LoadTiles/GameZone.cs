using System.Collections;
using System.Collections.Generic;
using System.Threading;
using UnityEngine;
using UnityEngine.Tilemaps;

public class GameZone : MonoBehaviour
{
    private static string m_terrainTilesPath = "Tiles/TerrainTiles/terrain_atlas";

    // Start is called before the first frame update
    void Start()
    {
        SetupTiles();
    }

    void SetupTiles()
    {
        Tilemap baseLevel = GetComponentsInChildren<Tilemap>()[0];
        baseLevel.SetTile(new Vector3Int(0, 0, 0), TilesResourcesLoader.GetTileByNameAndId("Tiles/TerrainTiles/terrain_atlas", 1));

        int[][] mat = new int[2][];
        mat[0] = new int[2] {0,20};
        mat[1] = new int[2] {20,30};
        LoadMatrix(mat);

        TCPTestClient.ConnectIfNotConnected();
    }

    private void Update()
    {
        if (Input.GetKeyDown(KeyCode.Space))
        {
            TCPTestClient.SendMessage("test message");
        }
    }

    public void LoadMatrix(int[][] matrix)
    {
        for(int i = 0; i < matrix.Length; ++i)
        {
            for(int j = 0; j < matrix[i].Length; ++j)
            {
                Tilemap baseLevel = GetComponentsInChildren<Tilemap>()[0];
                baseLevel.SetTile(new Vector3Int(i, -j, 1), TilesResourcesLoader.GetTileByNameAndId(m_terrainTilesPath,matrix[i][j]));
            }
        }
    }
}

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DynamicTilePrefab : MonoBehaviour
{
    public DynamicTile tile;

    private void Start()
    {
        tile = new DynamicTile();
    }
}

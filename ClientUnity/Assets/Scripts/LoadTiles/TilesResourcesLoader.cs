using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Tilemaps;

public class TilesResourcesLoader
{
    private const string Path1 = "horizontal";
    private const string Path2 = "start_stop";
    public static Tile GetPathHorizontalTile()
    {
        return GetTileByName(Path1);
    }
    public static Tile GetStartStopTile()
    {
        return GetTileByName(Path2);
    }

    private static Tile GetTileByName(string name)
    {
        return (Tile)Resources.Load(name, typeof(Tile));
    }

    public static Tile GetTileByNameAndId(string name, int id)
    {
        return GetTileByName(name + "_" + id);
    }
}

using SimpleJSON;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public static class GameLobbyData
{
	public static int GameId, PlayerId;
	public static int[] PlayersRoles;
	public static string MapName;
	public static string[] MapList;
	public static bool CreatedGame = false;
	public static int TotalNbRoles;

	public static JSONArray BlocksJson;
	public static JSONArray ObjectsJson;
	public static JSONArray PlayersJson;
	public static string LevelName;
}

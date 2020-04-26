using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MessageBuilders
{
	public static string BuildGameListMessage() 
	{
		return "{\"Action\":\"gameList\"}";
	}

	public static string BuildJoinGameMessage(int gameId) 
	{
		return "{\"Action\":\"joinGame\",\"GameId\":" + gameId + "}";
	}

	public static string BuildCreateGameMessage() {
		return "{\"Action\":\"createGame\"}";
	}

	public static string BuildChangeMapMessage(string map) 
	{
		return "{\"Action\":\"changeMap\",\"Map\":\"" + map + "\"}";
	}

	public static string BuildChangeRoleMessage(int role) 
	{
		return "{\"Action\":\"roleChange\",\"RoleId\":" + role + "}";
	}

	public static string BuildStartGameMessage() 
	{
		return "{\"Action\":\"startGame\"}";
	}

	public static string BuildMovementMessage(string direction)
    {
        return "{\"Action\":\"move\",\"Direction\":\"" + direction + "\"}";
    }

    public static string BuildActionMessage()
    {
        return "{\"Action\":\"action\"}";
    }
}

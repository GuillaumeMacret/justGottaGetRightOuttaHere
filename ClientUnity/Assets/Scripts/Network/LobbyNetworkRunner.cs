using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LobbyNetworkRunner : MonoBehaviour
{
	public LobbyMessageHandler handler;
    // Start is called before the first frame update
    void Start() 
	{
		TCPClient.ConnectIfNotConnected();
		TCPClient.SetMessageHandler(handler);
	}

	public void SendRoleChangeRequest(int roleId)
    {
		string message = MessageBuilders.BuildChangeRoleMessage(roleId);
		if (message != "")
			TCPClient.SendMessage(message);
    }

	public void SendGameStartRequest() 
	{
		string message = MessageBuilders.BuildStartGameMessage();
		if (message != "")
			TCPClient.SendMessage(message);
	}

	public void SendMapChangeRequest(int index) 
	{
		string message = MessageBuilders.BuildChangeMapMessage(GameLobbyData.MapList[index]);
		if (message != "")
			TCPClient.SendMessage(message);
	}

	public void SendLeaveGameRequest() 
	{
		string message = MessageBuilders.BuildLeaveGameMessage();
		if(message != "")
			TCPClient.SendMessage(message);
	}
}

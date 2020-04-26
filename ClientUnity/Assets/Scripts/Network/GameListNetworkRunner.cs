using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameListNetworkRunner : MonoBehaviour
{
	public GameListMessageHandler handler;
    // Start is called before the first frame update
    void Start()
    {
		TCPClient.ConnectIfNotConnected();
		TCPClient.SetMessageHandler(handler);
		SendGameListRequest();
	}

	public void SendGameListRequest() 
	{
		string messageToSend = MessageBuilders.BuildGameListMessage();
		if (messageToSend != "") {
			TCPClient.SendMessage(messageToSend);
		}
	}
}

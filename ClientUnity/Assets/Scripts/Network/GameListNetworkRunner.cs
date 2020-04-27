using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameListNetworkRunner : MonoBehaviour
{
	public GameListMessageHandler handler;
	public float TimeBetweenRefreshRequests = 5f;
	private float lastRefresh;
    // Start is called before the first frame update
    void Start()
    {
		TCPClient.ConnectIfNotConnected();
		TCPClient.SetMessageHandler(handler);
		lastRefresh = 0f;
		SendGameListRequest();
	}

	void Update() {
		lastRefresh += Time.deltaTime;
		if(lastRefresh > TimeBetweenRefreshRequests) 
		{
			SendGameListRequest();
		}
	}

	public void SendGameListRequest() 
	{
		string messageToSend = MessageBuilders.BuildGameListMessage();
		if (messageToSend != "") 
		{
			while(!TCPClient.IsSocketConnected());
			TCPClient.SendMessage(messageToSend);
			lastRefresh = 0f;
		}
	}

	public void SendGameCreateRequest() 
	{
		string messageToSend = MessageBuilders.BuildCreateGameMessage();
		if(messageToSend != "") 
		{
			TCPClient.SendMessage(messageToSend);
		}
	}
}

using System;
using System.Collections;
using System.Collections.Generic;
using SimpleJSON;
using UnityEngine;
using UnityEngine.UI;

public class LobbyManager : MonoBehaviour
{
	[Header("Roles")]
	public Sprite[] RoleCharacterSprites;
	public string[] RoleDescriptions;

	[Header("Players")]
	public LobbyPlayerController[] LobbyPlayerControllers;

	[Header("Game")]
	public Text GameIdText;
	public Button StartGameButton;

	[Header("Network")]
	public LobbyNetworkRunner Runner;

	[Header("Map")]
	public Dropdown MapsDropdown;
	public Image MapName;
	public Text MapText;

	// Private bool to change state of UI
	private bool changeMapName;
	private bool playerJoined;
	private int indexToUpdate;

    // Start is called before the first frame update
    void Start()
    {
		GameLobbyData.TotalNbRoles = RoleDescriptions.Length;
		if(GameLobbyData.CreatedGame) // Game created by user
		{
			MapName.gameObject.SetActive(false);
			MapsDropdown.AddOptions(new List<string>(GameLobbyData.MapList));
			MapsDropdown.onValueChanged.AddListener(delegate { MapChanged(MapsDropdown); });
			LobbyPlayerControllers[0].ChangeRole(0, RoleCharacterSprites[0], RoleDescriptions[0]);
		}
		else // Game not created by user
		{
			MapsDropdown.gameObject.SetActive(false);
			StartGameButton.interactable = false;
			ChangeMap(GameLobbyData.MapName);
			for(int i = 0; i < GameLobbyData.PlayersRoles.Length; ++i) 
			{
				LobbyPlayerControllers[i].ChangeRole(GameLobbyData.PlayersRoles[i], RoleCharacterSprites[GameLobbyData.PlayersRoles[i]], RoleDescriptions[GameLobbyData.PlayersRoles[i]]);
			}
		}
		// Removing change character button for other players
		for(int i = 0; i < LobbyPlayerControllers.Length; ++i) 
		{
			if (i != GameLobbyData.PlayerId)
				LobbyPlayerControllers[i].ChangeCharacterButton.gameObject.SetActive(false);
		}
    }

    // Update is called once per frame
    void Update()
    {
        if(changeMapName) 
		{
			MapText.text = GameLobbyData.MapName;
			changeMapName = false;
		}
		if(playerJoined) 
		{
			LobbyPlayerControllers[indexToUpdate].ChangeRole(GameLobbyData.PlayersRoles[indexToUpdate], RoleCharacterSprites[GameLobbyData.PlayersRoles[indexToUpdate]], RoleDescriptions[GameLobbyData.PlayersRoles[indexToUpdate]]);
			playerJoined = false;
		}
    }

	void MapChanged(Dropdown change) 
	{
		Runner.SendMapChangeRequest(change.value);
	}

	public void JoinedGame(int playerId, int[] roles) 
	{
		indexToUpdate = playerId;
		playerJoined = true;
	}

	public void ChangeMap(string newMap) 
	{
		GameLobbyData.MapName = newMap;
		changeMapName = true;
	}

	public void ChangeRole(int playerId, int roleId) 
	{
		if(roleId < RoleCharacterSprites.Length && roleId < RoleDescriptions.Length) 
		{
			GameLobbyData.PlayersRoles[playerId] = roleId;
			LobbyPlayerControllers[playerId].ChangeRole(roleId, RoleCharacterSprites[roleId], RoleDescriptions[roleId]);
		}
	}
}

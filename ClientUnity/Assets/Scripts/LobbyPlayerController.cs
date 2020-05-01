using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class LobbyPlayerController : MonoBehaviour
{
	public LobbyNetworkRunner lobbyNetworkRunner;

	public Sprite DefaultCharacterSprite;
	public Button PreviousCharacterButton;
	public Button NextCharacterButton;
	public Text PlayerIdText;
	public Text AbilityText;
	public Image CharacterImage;
	
	private int playerId;
	private int currentRole;
	private string ability;
	private bool changeRole;
	private Sprite characterSprite;

    void Awake()
    {
		ability = "Ability:";
		characterSprite = DefaultCharacterSprite;
		changeRole = true;
    }

	void Start() {
		PreviousCharacterButton.onClick.AddListener(ChangeToPreviousCharacter);
		NextCharacterButton.onClick.AddListener(ChangeToNextCharacter);
	}

	// Update is called once per frame
	void Update()
    {
        if(changeRole) 
		{
			if (characterSprite != null)
				CharacterImage.sprite = characterSprite;
			else
				CharacterImage.sprite = DefaultCharacterSprite;
			AbilityText.text = ability;
			changeRole = false;
		}
    }

	void ChangeToPreviousCharacter() 
	{
		if (lobbyNetworkRunner != null) 
		{
			if(currentRole - 1 < 0)
				lobbyNetworkRunner.SendRoleChangeRequest(GameLobbyData.TotalNbRoles - 1);
			else
				lobbyNetworkRunner.SendRoleChangeRequest(currentRole - 1);
		}
	}

	void ChangeToNextCharacter() {
		if (lobbyNetworkRunner != null)
			lobbyNetworkRunner.SendRoleChangeRequest((currentRole + 1) % GameLobbyData.TotalNbRoles);
	}

	public void ChangeRole(int roleId, Sprite newCharacterSprite, string newAbility) 
	{
		currentRole = roleId;
		characterSprite = newCharacterSprite;
		ability = newAbility;
		changeRole = true;
	}

	public void Reset() 
	{
		characterSprite = DefaultCharacterSprite;
		ability = "Ability:";
		changeRole = true;
	}
}

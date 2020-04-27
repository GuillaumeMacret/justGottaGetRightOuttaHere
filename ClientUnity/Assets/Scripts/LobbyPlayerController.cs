using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class LobbyPlayerController : MonoBehaviour
{
	public LobbyNetworkRunner lobbyNetworkRunner;

	public Sprite DefaultCharacterSprite;
	public Button ChangeCharacterButton;
	public Text PlayerIdText;
	public Text AbilityText;
	public Image CharacterImage;
	
	private int playerId;
	private int currentRole;
	private string ability;
	private bool changeRole;
	private Sprite characterSprite;
	private float timeBetweenRefresh = 1f;
	private float lastUpdate;

    // Start is called before the first frame update
    void Start()
    {
		ability = "Ability:";
		characterSprite = DefaultCharacterSprite;
		ChangeCharacterButton.onClick.AddListener(ChangeCharacter);
		changeRole = true;
		lastUpdate = 0f;
    }

    // Update is called once per frame
    void Update()
    {
		lastUpdate += Time.deltaTime;
        if(changeRole || lastUpdate > timeBetweenRefresh) {
			Debug.Log("Update Role:" + currentRole);
			Debug.Log(characterSprite);
			Debug.Log(ability);
			if (characterSprite != null)
				CharacterImage.sprite = characterSprite;
			else
				CharacterImage.sprite = DefaultCharacterSprite;
			AbilityText.text = ability;
			changeRole = false;
			lastUpdate = 0f;
		}
    }

	void ChangeCharacter() 
	{
		if (lobbyNetworkRunner != null)
			lobbyNetworkRunner.SendRoleChangeRequest((currentRole + 1) % GameLobbyData.TotalNbRoles);
	}

	public void ChangeRole(int roleId, Sprite newCharacterSprite, string newAbility) 
	{
		Debug.Log("Role:" + roleId);
		currentRole = roleId;
		characterSprite = newCharacterSprite;
		ability = newAbility;
		changeRole = true;
	}
}

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerAnimatorControllerFactory : MonoBehaviour
{

    public List<RuntimeAnimatorController> playerAnimators;

    public RuntimeAnimatorController GetAnimatorController(int id)
    {
        if (id > playerAnimators.Count - 1)
        {
            Debug.LogWarning("No animator found for id : " + id + ", returning the default one");
            return playerAnimators[0];
        }
        return playerAnimators[id];
    }
}

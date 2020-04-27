using System.Collections;
using System.Collections.Generic;
using UnityEditor.Animations;
using UnityEngine;

public class PlayerAnimatorControllerFactory : MonoBehaviour
{

    public List<AnimatorController> playerAnimators;

    public AnimatorController GetAnimatorController(int id)
    {
        if (id > playerAnimators.Count - 1)
        {
            Debug.LogWarning("No animator found for id : " + id + ", returning the default one");
            return playerAnimators[0];
        }
        return playerAnimators[id];
    }
}

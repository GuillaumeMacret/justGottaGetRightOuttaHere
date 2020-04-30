using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CrossSceneAudioSource : MonoBehaviour
{
    private static CrossSceneAudioSource Instance = null;
    public static CrossSceneAudioSource instance { get { return instance; } }


    private static AudioSource source = null;
    private void Awake()
    {
        source = GetComponent<AudioSource>();
        if(Instance != null && Instance != this)
        {
            Destroy(this.gameObject);
            return;
        }
        else
        {
            Instance = this;
        }
        DontDestroyOnLoad(this.gameObject);
    }

    public static void PlayClip(AudioClip audioClip)
    {
        if(source == null)
        {
            Debug.LogError("No cross scene audio source was found!");
            return;
        }
        source.clip = audioClip;
        source.Play();
    }
}

using System.Collections.Generic;
using UnityEngine;
using UnityEditor.Animations;

public class Player : MonoBehaviour
{
    public const int ROLE_PHANTOM = 1;

    public int id;
    public int speed = 10;
    public List<AudioClip> stepSounds;
    public float pitchRangeDown = .8f;
    public float pitchRangeUp = 1.2f;

    private Animator m_animator;
    private AudioSource m_audioSource;
    private SpriteRenderer m_SpriteRenderer;

    private Vector3 lastDirection;
    public List<Vector3> targetPositions;

    private void Awake()
    {
        m_animator = GetComponent<Animator>();
        m_audioSource = GetComponent<AudioSource>();
        m_SpriteRenderer = GetComponent<SpriteRenderer>();
    }

    private void MoveTowardsNextDestination()
    {
        Debug.Log("Player " + id + " Moving toward " + targetPositions[0]);
        float step = speed * Time.deltaTime;
        transform.position = Vector3.MoveTowards(transform.position, targetPositions[0], step);

        if(transform.position == targetPositions[0])
        {
            targetPositions.RemoveAt(0);
        }
    }

    private void Update()
    {
        m_SpriteRenderer.enabled = true;
        speed = 10;
        if (targetPositions.Count > 0)
        {
            //Try to prevent pivot when bumping into a wall
            if (transform.position == targetPositions[0])
            {
                targetPositions.RemoveAt(0);
                return;
            }
            else
            {

                Vector3 direction = targetPositions[0] - transform.position;

                if (direction.magnitude > 3)
                {
                    Debug.Log("Big movement detected, going sneaky mode");
                    speed = 100;
                    m_SpriteRenderer.enabled = false;
                }

                MoveTowardsNextDestination();
                m_animator.SetBool("Moving", true);
                if (!m_audioSource.isPlaying && id != ROLE_PHANTOM)
                {
                    m_audioSource.pitch = Random.Range(pitchRangeDown, pitchRangeUp);
                    m_audioSource.PlayOneShot(stepSounds[Random.Range(0, stepSounds.Count)]);
                }
                lastDirection = direction;
            }
        }
        else
        {
            m_animator.SetBool("Moving", false);
        }

        m_animator.SetFloat("MoveX", lastDirection.x);
        m_animator.SetFloat("MoveY", lastDirection.y);

    }

    public void AddDestination(int xPos, int yPos)
    {
        targetPositions.Add(new Vector3(xPos, yPos, 0));
    }

    public void SetAnimatorController(AnimatorController anim)
    {
        if(m_animator == null)
        {
            Debug.LogError("Player's Animator reference is null! Can't set the controller ");
            return;
        }
        m_animator.runtimeAnimatorController = anim;
    }
}

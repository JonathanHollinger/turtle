#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "effects.h"
#include "model.h"

static state_t const _transitions[NUM_STATES][NUM_EVENTS] = {
  // Token, Pipe, Newline
  { Command, Term, NST },         // Init
  { Arguments, Make_Pipe, Term }, // Command
  { Arguments, Make_Pipe, Term }, // Args
  { Command, Term, Term },        // Pipe
  { NST, NST, NST }               // NIL
};

static action_t const _effects[NUM_STATES][NUM_EVENTS] = {
  // Token, Pipe, Newline
  { start_command, error_pipe, NULL },          // Init
  { append, link_commands, execute },           // Command
  { append, link_commands, execute },           // Arguments
  { start_command, error_pipe, error_newline }, // Pipe
  { NULL, NULL, NULL }                          // NIL
};

state_t
transition_get (struct fsm *fsm, event_t event, action_t *action)
{
  // Look up the next state from the transition table based on the current
  // state and event
  state_t new_state = _transitions[fsm->state][event];

  // Look up the associated action from the effect table
  *action = _effects[fsm->state][event]; // Set the action by reference

  // Return the new state
  return new_state;
}

// TODO: Create a table mapping states/events to the effect functions. If
// there is no valid transition, the entry here would be NULL because actions
// are function pointers.

/* Create an instance of an FSM and initialize its fields as appropriate.
   Some fields are common to most FSMs (such as an initial state or a
   pointer to a transition function). Other fields will be specific to
   this fsm_t declaration. Return NULL if any part of the initialization
   fails. */
fsm_t *
cmdline_init (void)
{
  fsm_t *fsm = (fsm_t *)calloc (1, sizeof (fsm_t));
  if (fsm == NULL)
    {
      return NULL; // Memory allocation failed
    }

  // Set the initial state
  fsm->state = Init;
  fsm->transition = transition_get;

  return fsm;
}

// Creates a transition function that is specific to this type
// of FSM. This function needs to take an fsm_t* and an event, returning
// both the new state and the effect to perform (the latter is returned
// using a call-by-reference parameter. This function should NOT contain
// any "if" types of statements based on the state or event; it should
// simply lookup these values in the tables defined above.

const char *
event_name (event_t evt)
{
  assert (evt <= NIL);

  // Event names for printing out
  const char *names[] = { "TOKEN", "PIPE", "NEWLINE", "NIL" };
  return names[evt];
}

/* Helper function for providing a printable string name for a state */
const char *
state_name (state_t st)
{
  assert (st <= NST);

  // State names for printing out
  const char *names[]
      = { "Init", "Command", "Arguments", "Make_Pipe", "Term", "NST" };
  return names[st];
}

/* Generic front-end for handling events. Should do nothing more
   than calling the FSM's transition function, performing an effect
   (if appropriate) and updating the state. Return false if the new
   state is the terminal state. */
bool
handle_event (fsm_t *fsm, event_t event)
{

  // Asserts stop the current command if not true
  assert (fsm != NULL);
  assert (fsm->state < NUM_STATES); // Validate the state is within valid range
  assert (event < NUM_EVENTS);

  // Look up the transition table for the next state
  state_t next_state = _transitions[fsm->state][event];

  // If there's no valid transition, return false
  if (next_state == NST)
    {
      return false;
    }

  action_t effect = _effects[fsm->state][event];

  // If there is an effect, perform the effect
  if (effect != NULL)
    {
      effect (fsm);
      // Execute the effect (such as appending arguments, etc.)
    }
  // Otherwise, update the FSM state and return true
  fsm->state = next_state;
  return true;
}

/* Given a string, return the event type. Do not modify this function. */
event_t
lookup (char *token)
{
  if (!strcmp (token, "|"))
    return PIPE;

  if (!strcmp (token, "NL"))
    return NEWLINE;

  return TOKEN;
}

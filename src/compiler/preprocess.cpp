#include "preprocess.hpp"

using std::vector;
using std::string;

enum PreprocessState {STATE_START, 
                      STATE_STRING,
                      STATE_STRING_ESCAPE,
                      STATE_COMMENT,
                      STATE_BLOCK_COMMENT,
                      STATE_BLOCK_COMMENT_END};

void Preprocess::removeComments (vector<string> &source_code)
{
  int curr_state = STATE_START;

  // Iterate over the source code line by line 
  for (int curr_line_index = 0; 
       curr_line_index < source_code.size (); curr_line_index++)
  {
    string &curr_line (source_code[curr_line_index]);

    // Iterate over the current line character by character 
    for (int curr_char_index = 0; 
         curr_char_index < curr_line.length (); curr_char_index++)
    {

      char curr_char = curr_line[curr_char_index];

      switch (curr_state)
      {
        case STATE_START:
          if (curr_char == '"')
            curr_state = STATE_STRING;
          else if (curr_char == '/')
            curr_state = STATE_COMMENT;
          break;

        case STATE_STRING:
          if (curr_char == '"')
            curr_state = STATE_START;
          else if (curr_char == '\\')
            curr_state = STATE_STRING_ESCAPE;
          break;

        case STATE_STRING_ESCAPE:
          curr_state = STATE_STRING;
          break;

        case STATE_COMMENT:
          // line comment:
          // take substring from index 0 to right before '//'
          if (curr_char == '/')
          {
            curr_line = curr_line.substr (0, curr_char_index - 1);
            curr_line += "\n";
            curr_state = STATE_START;
          }
          // block comment:
          // replace any character inside block with blank space
          else if (curr_char == '*')
          {
            curr_line[curr_char_index - 1] = ' ';
            curr_line[curr_char_index] = ' ';
            curr_state = STATE_BLOCK_COMMENT;
          }
          else
            curr_state = STATE_START;
          break;
        
        case STATE_BLOCK_COMMENT:
          if (curr_char == '*')
            curr_state = STATE_BLOCK_COMMENT_END;
          curr_line[curr_char_index] = ' ';
          break;

        case STATE_BLOCK_COMMENT_END:
          if (curr_char == '/')
            curr_state = STATE_START;
          else
            curr_state = STATE_BLOCK_COMMENT;
          curr_line[curr_char_index] = ' ';
          break;
      }
    }
  }
}

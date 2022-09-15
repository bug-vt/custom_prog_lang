#ifndef _LABEL_TABLE
#define _LABEL_TABLE

#include <string>
#include <unordered_map>

struct Label
{
  std::string ident;
  int func_index;

  Label () { }
  Label (std::string ident, int func_index);
  bool operator== (const Label &other) const
  {
    return ident == other.ident && func_index == other.func_index;
  }
};

// To use user-defined key type for unordered_map,
// hash function must overrides operator() and calculates the hash value
// for a given key type.
namespace std 
{
  template <>
  struct hash<Label>
  {
    std::size_t operator() (const Label &label) const
    {
      using std::size_t;
      using std::hash;
      using std::string;

      return ((hash<string>()(label.ident)
              ^ (hash<int>()(label.func_index) << 1)) >> 1);
    }
  };
}

struct LabelInfo
{
  int label_index;    // index within the label table
  int target_index;   // index of the target instruction 

  LabelInfo () { }
  LabelInfo (int index, int target);
};


class LabelTable
{
  public:
    LabelTable ();
    int addLabel (Label label, int target_index);
    LabelInfo getLabel (Label label);
    int getTargetIndex (Label label);
    void print ();

  private:
    int label_count;
    std::unordered_map<Label, LabelInfo> label_table;
};


#endif

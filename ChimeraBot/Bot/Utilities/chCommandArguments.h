#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>

class CmdParser {
public:
  static CmdParser& getInstance() {
    static CmdParser instance;
    return instance;
  }

  CmdParser(CmdParser const&) = delete;
  void operator=(CmdParser const&) = delete;

  void parse(int argc, char** argv) {
    for (int i = 1; i < argc; ++i) {
      std::string arg = argv[i];
      std::string lowerArg = arg;
      std::transform(lowerArg.begin(), lowerArg.end(), lowerArg.begin(),
        [](unsigned char c) { return std::tolower(c); });

      size_t pos = lowerArg.find('=');
      if (pos == std::string::npos) {
        flags.insert(lowerArg.substr(1));
      }
      else {
        params[lowerArg.substr(1, pos - 1)] = arg.substr(pos + 1);
      }
    }
  }

  std::string getParam(const std::string& param, const std::string defaultValue = "") {
    std::string lowerParam = param;
    std::transform(lowerParam.begin(), lowerParam.end(), lowerParam.begin(),
      [](unsigned char c) { return std::tolower(c); });

    auto it = params.find(lowerParam);
    return (it != params.end()) ? it->second : defaultValue;
  }

  bool isFlagSet(const std::string& flag) {
    std::string lowerFlag = flag;
    std::transform(lowerFlag.begin(), lowerFlag.end(), lowerFlag.begin(),
      [](unsigned char c) { return std::tolower(c); });

    return flags.find(lowerFlag) != flags.end();
  }

private:
  CmdParser() = default;
  std::unordered_map<std::string, std::string> params;
  std::unordered_set<std::string> flags;
};

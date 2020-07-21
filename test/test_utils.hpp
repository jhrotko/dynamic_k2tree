#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <string>
#include <vector>

void split(const std::string& str, std::vector<std::string>& cont,
           const std::string& delims = " ") {
    boost::split(cont, str, boost::is_any_of(delims));
}

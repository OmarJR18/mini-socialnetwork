#ifndef COMMENT_HPP
#define COMMENT_HPP

#include <string>

class Comment {
private:
    int m_id;
    int m_author_id;
    int m_post_id;
    std::string m_text;
    std::string m_date;

public:
    Comment(int _id = 0, int _author_id = 0, int _post_id = 0, const std::string& _text = "", const std::string& _date = "");

    int id() const;
    int author_id() const;
    int post_id() const;
    const std::string& text() const;
    const std::string& date() const;
};

#endif // COMMENT_HPP

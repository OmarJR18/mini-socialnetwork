#include <iostream>

#include <modelo/Comment.hpp>
#include <modelo/Post.hpp>
#include <modelo/User.hpp>

int main()
{
    std::cout << "=== Prueba de User, Post y Comment ===" << std::endl;

    User user(1, "Ana Torres", "ana@mail.com", "2026-07-01");
    user.add_friend_id(2);
    user.add_friend_id(3);
    user.add_post_id(100);

    std::cout << "Usuario: " << user.name() << " (" << user.email() << ")" << std::endl;
    std::cout << "Amigos: " << user.friend_count() << std::endl;
    std::cout << "Publicaciones: " << user.post_count() << std::endl;

    Post post(100, 1, "2026-07-02", "Mi primera publicacion");
    post.add_like();
    post.add_like();

    Comment comment(200, 2, 100, "Que buena publicacion!", "2026-07-02");
    post.add_comment_id(comment.id());

    std::cout << "Post de " << post.owner_id() << ": \"" << post.text() << "\"" << std::endl;
    std::cout << "Likes: " << post.like_count() << ", Comentarios: " << post.comment_count() << std::endl;
    std::cout << "Comentario de " << comment.author_id() << ": " << comment.text() << std::endl;

    user.remove_friend_id(2);
    std::cout << "Amigos tras remove: " << user.friend_count() << std::endl;

    return 0;
}

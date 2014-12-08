#include "DatabaseManager.h"


/**
 * @brief Adds a movie to the database
 *
 * @param Movie
 * @return bool
 */
bool DatabaseManager::insertNewMovie(Movie &movie)
{
    QSqlQuery l_query(m_db);
    l_query.prepare("INSERT INTO movies(title, original_title, release_date, country, duration, synopsis, file_path, colored, format, suffix, rank) "
                    "VALUES (:title, :original_title, :release_date, :country, :duration, :synopsis, :file_path, :colored, :format, :suffix, :rank)");
    l_query.bindValue(":title", movie.getTitle());
    l_query.bindValue(":original_title", movie.getOriginalTitle()   );
    l_query.bindValue(":release_date", movie.getReleaseDate().toString(DATE_FORMAT));
    l_query.bindValue(":country", movie.getCountry());
    l_query.bindValue(":duration", movie.getDuration().msecsSinceStartOfDay());
    l_query.bindValue(":synopsis", movie.getSynopsis());
    l_query.bindValue(":file_path", movie.getFilePath());
    l_query.bindValue(":colored", movie.isColored());
    l_query.bindValue(":format", movie.getFormat());
    l_query.bindValue(":suffix", movie.getSuffix());
    l_query.bindValue(":rank", movie.getRank());

    if (!l_query.exec())
    {
        qDebug() << "In insertNewMovie():";
        qDebug() << l_query.lastError().text();

        return false;
    }

    qDebug() << "[DatabaseManager] Movie added";

    movie.setId(l_query.lastInsertId().toInt());

    for(int i = 0 ; i < movie.getPeopleList().size() ; i++)
    {
        People l_people = movie.getPeopleList().at(i);
        if (!addPeopleToMovie(l_people, movie, l_people.getType()))
        {
            return false;
        }
    }

    return true;
}

/**
 * @brief Adds a person to the database and links it to a movie
 *
 * @param People
 * @param Movie
 * @return bool
 */
bool DatabaseManager::addPeopleToMovie(People &people, Movie &movie, int type)
{
    if (!insertNewPeople(people))
    {
        return false;
    }
    QSqlQuery l_query(m_db);
    l_query.prepare("INSERT INTO movies_people (id_people, id_movie, type) "
                    "VALUES (:id_people, :id_movie, :type)");
    l_query.bindValue(":id_people", people.getId());
    l_query.bindValue(":id_movie", movie.getId());
    l_query.bindValue(":type", type);

    if (!l_query.exec())
    {
        qDebug() << "In addPeopleToMovie():";
        qDebug() << l_query.lastError().text();

        return false;
    }

    movie = getOneMovieById(movie.getId());

    return true;
}

/**
 * @brief Adds a tag to the database and links it to a movie
 *
 * @param Tag
 * @param Movie
 * @return bool
 */
bool DatabaseManager::addTagToMovie(Tag &tag, Movie &movie)
{
    if (!insertNewTag(tag))
    {
        return false;
    }

    QSqlQuery l_query(m_db);
    l_query.prepare("INSERT INTO movies_tags (id_tag, id_movie) "
                    "VALUES (:id_tag, :id_movie)");
    l_query.bindValue(":id_tag", tag.getId());
    l_query.bindValue(":id_movie", movie.getId());

    if (!l_query.exec())
    {
        qDebug() << "In addTagToMovie():";
        qDebug() << l_query.lastError().text();

        return false;
    }
    movie = getOneMovieById(movie.getId());

    return true;
}

/**
 * @brief Adds a person to the database.
 * Should not be called directly.
 *
 * @param People
 * @return bool
 */
bool DatabaseManager::insertNewPeople(People &people)
{
    QSqlQuery l_query(m_db);
    l_query.prepare("INSERT INTO people (lastname, firstname, realname, birthday, biography) "
                    "VALUES (:lastname, :firstname, :realname, :birthday, :biography)");
    l_query.bindValue(":lastname", people.getLastname());
    l_query.bindValue(":firstname", people.getFirstname());
    l_query.bindValue(":realname", people.getRealname());
    l_query.bindValue(":birthday", people.getBirthday().toString(DATE_FORMAT));
    l_query.bindValue(":biography", people.getBiography());

    if (!l_query.exec())
    {
        qDebug() << "In insertNewPeople():";
        qDebug() << l_query.lastError().text();

        return false;
    }
    people.setId(l_query.lastInsertId().toInt());

    return true;
}

/**
 * @brief Adds a tag to the database.
 * Should not be called directly.
 *
 * @param Tag
 * @return bool
 */
bool DatabaseManager::insertNewTag(Tag &tag)
{
    QSqlQuery l_query(m_db);
    l_query.prepare("INSERT INTO tag (name) "
                    "VALUES (:name)");
    l_query.bindValue(":name", tag.getName());

    if (!l_query.exec())
    {
        qDebug() << "In insertNewTag():";
        qDebug() << l_query.lastError().text();

        return false;
    }
    tag.setId(l_query.lastInsertId().toInt());

    return true;
}

/**
 * @brief Adds a playlist to the database.
 *
 * @param Playlist
 * @return bool
 */
bool DatabaseManager::insertNewPlaylist(Playlist &playlist)
{
    QSqlQuery l_query(m_db);
    l_query.prepare("INSERT INTO playlists (name, creation_date, rate) "
                    "VALUES (:name, :creation_date, :rate)");
    l_query.bindValue(":name", playlist.getName());
    l_query.bindValue(":creation_date", playlist.getCreationDate().toTime_t());
    l_query.bindValue(":rate", playlist.getRate());

    if (!l_query.exec())
    {
        qDebug() << "In insertNewPlaylist():";
        qDebug() << l_query.lastError().text();

        return false;
    }
    playlist.setId(l_query.lastInsertId().toInt());

    return true;
}

/**
 * @brief Adds a movie to the database and links it to a playlist
 *
 * @param Movie
 * @param Playlist
 * @return bool
 */
bool DatabaseManager::addMovieToPlaylist(Movie &movie, Playlist &playlist)
{
    if (playlist.getId() == 0)
    {
        if (!insertNewPlaylist(playlist))
        {
            return false;
        }
    }

    QSqlQuery l_query(m_db);
    l_query.prepare("INSERT INTO movies_playlists (id_movie, id_playlist) "
                    "VALUES (:id_movie, :id_playlist)");
    l_query.bindValue(":id_movie", movie.getId());
    l_query.bindValue(":id_playlist", playlist.getId());

    if (!l_query.exec())
    {
        qDebug() << "In addMovieToPlaylist():";
        qDebug() << l_query.lastError().text();

        return false;
    }
    playlist = getOnePlaylistById(playlist.getId());

    return true;
}

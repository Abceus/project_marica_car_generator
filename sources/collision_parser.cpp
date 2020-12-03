#include "collision_parser.h"

#include <QRegExp>
#include <iostream>


ASEFile CollisionParser::parse(const QString& ase)
{
    ASEFile result;
    int currentGameObjectIndex = 0;
    int currentGameObjectIndexNext = 0;
    QRegExp gameObjectRegex( "GEOMOBJECT" );

    while( ( currentGameObjectIndex = gameObjectRegex.indexIn( ase, currentGameObjectIndexNext ) ) != -1 ) {
        ASEFile::GameObject gameObject;
        currentGameObjectIndexNext = currentGameObjectIndex + gameObjectRegex.matchedLength();

        gameObject.name = parseString(ase, "NODE_NAME", currentGameObjectIndexNext ).value;

        gameObject.tm_row0 = parseVertex(ase, "TM_ROW0", currentGameObjectIndexNext ).value;
        gameObject.tm_row1 = parseVertex(ase, "TM_ROW1", currentGameObjectIndexNext ).value;
        gameObject.tm_row2 = parseVertex(ase, "TM_ROW2", currentGameObjectIndexNext ).value;
        gameObject.tm_row3 = parseVertex(ase, "TM_ROW3", currentGameObjectIndexNext ).value;

        gameObject.tm_rotaxis = parseVertex(ase, "TM_ROTAXIS", currentGameObjectIndexNext ).value;
        gameObject.tm_rotangle = parseFloat(ase, "TM_ROTANGLE", currentGameObjectIndex).value;

        gameObject.tm_pos = parseVertex(ase, "TM_POS", currentGameObjectIndexNext ).value;

        gameObject.mesh.vertices.resize(parseInt( ase, "MESH_NUMVERTEX", currentGameObjectIndexNext ).value);

        auto vertices = parseArray(ase,  "MESH_VERTEX", "MESH_VERTEX_LIST", currentGameObjectIndexNext);
        for(int i = 0; i < vertices.value.size(); i++)
        {
            gameObject.mesh.vertices[i] = parseVertex(vertices.value[i], "").value;
        }

        gameObject.mesh.faces.resize(parseInt( ase, "MESH_NUMFACES", currentGameObjectIndexNext ).value);

        auto faces = parseArray(ase, "MESH_FACE", "MESH_FACE_LIST", currentGameObjectIndexNext);
        for(int i = 0; i < faces.value.size(); i++)
        {
            gameObject.mesh.faces[i] = parseFace(faces.value[i], "").value;
        }

        result.gameObjects.append(gameObject);
    }
    return result;
}

CollisionParser::ValueOffset<QString> CollisionParser::parseString(const QString& text, const QString& token, int offset)
{
    if(token != "")
    {
        QRegExp nameTokenRegex( token );
        offset = nameTokenRegex.indexIn( text, offset );
        offset += nameTokenRegex.matchedLength();
    }

    QRegExp nameRegex( "[^\"\\s]+" );
    auto lastIndex = nameRegex.indexIn( text, offset );
    return {nameRegex.cap(), lastIndex};
}

CollisionParser::ValueOffset<int> CollisionParser::parseInt(const QString& text, const QString& token, int offset)
{
    if(token != "")
    {
        QRegExp nameTokenRegex( token );
        offset = nameTokenRegex.indexIn( text, offset );
        offset += nameTokenRegex.matchedLength();
    }

    QRegExp nameRegex( "\\d+" );
    auto lastIndex = nameRegex.indexIn( text, offset );
    return {nameRegex.cap().toInt(), lastIndex};
}

CollisionParser::ValueOffset<QVector<QString>> CollisionParser::parseArray(const QString& text, const QString& elemenToken, const QString& token, int offset)
{
    if(token != "")
    {
        QRegExp nameTokenRegex( token );
        offset = nameTokenRegex.indexIn( text, offset );
        offset += nameTokenRegex.matchedLength();
    }

    QRegExp openRegex( "\\{" );
    QRegExp closeRegex( "\\}" );
    int count = 1;
    int firstIndex = openRegex.indexIn( text, offset );
    firstIndex += openRegex.matchedLength();
    int lastIndex = firstIndex;

    QVector<QString> result;

    while(count != 0)
    {
        auto openIndex = openRegex.indexIn( text, lastIndex );
        if(openIndex == -1)
        {
            openIndex = text.size();
        }
        auto closeIndex = closeRegex.indexIn( text, lastIndex );
        if(closeIndex == -1)
        {
            openIndex = text.size();
        }
        if(openIndex < closeIndex)
        {
            lastIndex = openIndex + openRegex.matchedLength();
            count++;
        }
        else
        {
            lastIndex = closeIndex + closeRegex.matchedLength();
            count--;
        }
    }

    auto subplace = text.mid(firstIndex, lastIndex-firstIndex);

    int currentElementIndex = 0;

    QRegExp elementTokenRegex(elemenToken);

    while( ( currentElementIndex = elementTokenRegex.indexIn( subplace, currentElementIndex ) ) != -1 ) {
        currentElementIndex += elementTokenRegex.matchedLength();

        QRegExp intRegex( "\\d+" );
        currentElementIndex = intRegex.indexIn( subplace, currentElementIndex );
        currentElementIndex += intRegex.matchedLength();
        int index = intRegex.cap().toInt();

        auto nextElement = elementTokenRegex.indexIn( subplace, currentElementIndex );
        if(nextElement == -1)
        {
            nextElement = subplace.size();
        }

        if(result.size() < index+1)
        {
            result.resize(index+1);
        }

        result[index] = subplace.mid(currentElementIndex, nextElement-currentElementIndex);
    }

    return {result, lastIndex};
}

CollisionParser::ValueOffset<Vector3D> CollisionParser::parseVertex(const QString& text, const QString& token, int offset)
{
    if(token != "")
    {
        QRegExp nameTokenRegex( token );
        offset = nameTokenRegex.indexIn( text, offset );
        offset += nameTokenRegex.matchedLength();
    }

    QRegExp floatRegex( "[\\d-\\.]+" );
    Vector3D result;

    offset = floatRegex.indexIn(text, offset);
    result.setX(floatRegex.cap().toFloat());
    offset += floatRegex.matchedLength();

    offset = floatRegex.indexIn(text, offset);
    result.setY(floatRegex.cap().toFloat());
    offset += floatRegex.matchedLength();

    offset = floatRegex.indexIn(text, offset);
    result.setZ(floatRegex.cap().toFloat());
    offset += floatRegex.matchedLength();

    return {result, offset};
}

CollisionParser::ValueOffset<float> CollisionParser::parseFloat(const QString& text, const QString& token, int offset)
{
    if(token != "")
    {
        QRegExp nameTokenRegex( token );
        offset = nameTokenRegex.indexIn( text, offset );
        offset += nameTokenRegex.matchedLength();
    }

    QRegExp floatRegex( "[\\d-\\.]+" );
    float result;

    offset = floatRegex.indexIn(text, offset);
    result = floatRegex.cap().toFloat();
    offset += floatRegex.matchedLength();

    return {result, offset};
}

CollisionParser::ValueOffset<ASEFile::Face> CollisionParser::parseFace(const QString& text, const QString& token, int offset)
{
    ASEFile::Face result;

    if(token != "")
    {
        QRegExp nameTokenRegex( token );
        offset = nameTokenRegex.indexIn( text, offset );
        offset += nameTokenRegex.matchedLength();
    }

    QRegExp aTokenRegex( "A:" );
    offset = aTokenRegex.indexIn(text, offset);
    offset += aTokenRegex.matchedLength();

    auto a = parseInt(text, "", offset);
    result.A = a.value;
    offset = a.offset;

    QRegExp bTokenRegex( "B:" );
    offset = bTokenRegex.indexIn(text, offset);
    offset += bTokenRegex.matchedLength();

    auto b = parseInt(text, "", offset);
    result.B = b.value;
    offset = b.offset;

    QRegExp cTokenRegex( "C:" );
    offset = cTokenRegex.indexIn(text, offset);
    offset += cTokenRegex.matchedLength();

    auto c = parseInt(text, "", offset);
    result.C = c.value;
    offset = c.offset;

    return {result, offset};
}

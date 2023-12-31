#include "render_system.h"

RenderSystem::RenderSystem(sf::RenderTarget& renderTarget, EntityManager& entityManager)
    : m_renderTarget(renderTarget), m_entityManager(entityManager)
{
    configureTextRendering();
}

void RenderSystem::execute()
{
    drawEntities();
    drawGuiData();
}

void RenderSystem::drawEntities()
{
    std::vector<Entity> entities = m_entityManager.getEntities();
    for (const Entity e : entities)
    {
        if (e.hasComponent<Component::CLightSource>())
        {
            auto& cLightSource = e.getComponent<Component::CLightSource>();
            sf::VertexArray& lightVertices = cLightSource.lightVertices;
            m_renderTarget.draw(&lightVertices[0], lightVertices.getVertexCount(), sf::TriangleFan);
            //m_renderTarget.draw(&lightVertices[0], lightVertices.getVertexCount(), sf::Lines);
        }

        if (e.hasComponent<Component::CShape>())
        {
            auto& cShape = e.getComponent<Component::CShape>();
            m_renderTarget.draw(cShape.vertices);
        }
    }
}

void RenderSystem::drawGuiData()
{

}

// TODO move to engine for inter-scene text drawing?
void RenderSystem::drawText(sf::Text& text, const sf::Color& fillColour, const uint8_t characterSize, sf::Vector2f position)
{
    text.setFillColor(fillColour);
    text.setCharacterSize(characterSize); // in pixels, not points!
    text.setPosition(position);

    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(1.6f);
    text.setLetterSpacing(3.0f);

    m_renderTarget.draw(text);
}

void RenderSystem::configureTextRendering()
{
    bool isFontLoaded = m_font.loadFromFile(FONT_PATH);
    assert(isFontLoaded);

    m_ammoText = sf::Text("", m_font);
}
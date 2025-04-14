#include <locale.h>
#include <time.h>
#include <unistd.h>

#include <SFML/Graphics.hpp>
#include <interface.hpp>
#include <memory>
#include <network.hpp>

class GameController {
   public:
    GameController() {
        srand(time(NULL));

        window_ = std::make_unique<sf::RenderWindow>(
            sf::VideoMode(600, 640),
            "Tic-tac-toe");

        ipList_ = {"127.0.0.1"};
    }

    void run() {
        while (window_->isOpen()) {
            processEvents();
            update();
            render();
            // usleep(5000);
        }
    }

   private:
    // Game state
    std::unique_ptr<sf::RenderWindow> window_;
    StartWindow startWindow_;
    GameWindow gameWindow_;
    ResultWindow resultWindow_;

    Player player_;
    Player enemy_;
    NetworkManager networkManager_;

    Move currentMove_ = Move::Waiting;
    Result gameResult_;
    std::vector<std::string> ipList_;

    // Event processing
    void processEvents() {
        sf::Event event;
        sf::Vector2i mousePos = sf::Mouse::getPosition(*window_);

        while (window_->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window_->close();
                return;
            }

            handleMouseEvents(event, mousePos);
        }
    }

    void handleMouseEvents(const sf::Event& event, const sf::Vector2i& mousePos) {
        if (event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left) {
            if (startWindow_.isActive()) {
                handleStartWindowClick(mousePos);
            } else if (gameWindow_.isActive() && currentMove_ == Move::Player) {
                handleGameWindowClick(mousePos);
            } else if (resultWindow_.isActive()) {
                handleResultWindowClick(mousePos);
            }
        }
    }

    // Window-specific handlers
    void handleStartWindowClick(const sf::Vector2i& mousePos) {
        for (int i = 0; i < 3; i++) {
            if (startWindow_.getAllObjects()[i]->sprite.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                setupPlayerFigure(i);
                initializeNetwork();
                switchToGameWindow();
                break;
            }
        }
    }

    void setupPlayerFigure(int choice) {
        if (choice == 2) {
            player_.setFigure(static_cast<Figure>(rand() % 2 + 1));
        } else {
            player_.setFigure(static_cast<Figure>(choice + 1));
        }
    }

    void initializeNetwork() {
        networkManager_.setIpList(ipList_);
        networkManager_.setPort(PORT);
        networkManager_.setMyFigure(player_.getFigure());
        networkManager_.initialize();
    }

    void switchToGameWindow() {
        startWindow_.setActive(false);
        gameWindow_.setActive(true);
        gameWindow_.clearCells();
        gameWindow_.setPlayerFigure(player_.getFigure());
        gameWindow_.setWaitMode(true);
        gameWindow_.changeStepString(Move::Waiting);
    }

    void handleGameWindowClick(const sf::Vector2i& mousePos) {
        for (int i = 0; i < 9; i++) {
            auto& cell = gameWindow_.getCells()[i];
            if (cell.sprite.getGlobalBounds().contains(mousePos.x, mousePos.y) &&
                cell.figure == Figure::Empty) {
                makePlayerMove(i);
                break;
            }
        }
    }

    void makePlayerMove(int cellIndex) {
        gameWindow_.setCell(cellIndex, player_.getFigure());

        gameResult_ = gameWindow_.checkResult(gameWindow_, gameWindow_.getCells());
        if (gameResult_.winner != Winner::Unknown) {
            endGame();
        } else {
            currentMove_ = Move::Enemy;
            gameWindow_.changeStepString(currentMove_);
        }

        sendMove(cellIndex);
    }

    void sendMove(int cellIndex) {
        uint8_t move = static_cast<uint8_t>(cellIndex);
        send(networkManager_.getSocket(), &move, 1, 0);
    }

    void endGame() {
        gameWindow_.changeLinePosition(gameResult_);
        gameWindow_.changeStepString(Move::EndGame);
        resultWindow_.setActive(true);
    }

    void handleResultWindowClick(const sf::Vector2i& mousePos) {
        if (mousePos.y < 30) return;

        auto* clickedObject = resultWindow_.getAllObjects()[0];
        if (clickedObject->sprite.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            returnToMainMenu();
        } else if (networkManager_.getSocket() == -1) {
            attemptReconnect();
        } else {
            sendReadySignal();
        }
    }

    void returnToMainMenu() {
        networkManager_.disconnect();
        resultWindow_.setActive(false);
        startWindow_.setActive(true);
    }

    void attemptReconnect() {
        networkManager_.initialize();
        resultWindow_.setActive(false);
        gameWindow_.setActive(true);
        gameWindow_.clearCells();
        gameWindow_.setWaitMode(true);
        gameWindow_.changeStepString(Move::Waiting);
        networkManager_.setReadyToContinue(false);
        networkManager_.setEnemyReady(false);
    }

    void sendReadySignal() {
        networkManager_.setReadyToContinue(true);
        uint8_t msg = 100;  // Ready signal
        send(networkManager_.getSocket(), &msg, 1, 0);
    }

    // Game state updates
    void update() {
        if (gameWindow_.getWaitMode()) {
            updateWaitingState();
        }

        if (gameWindow_.isActive() && currentMove_ == Move::Enemy && !resultWindow_.isActive()) {
            processEnemyMove();
        }

        if (resultWindow_.isActive() && networkManager_.getSocket() != -1) {
            checkReadyStatus();
        }
    }

    void updateWaitingState() {
        if (networkManager_.isReady()) {
            enemy_.setFigure(networkManager_.getOpponentFigure());
            gameWindow_.setWaitMode(false);
            gameWindow_.setEnemyFigure(enemy_.getFigure());

            currentMove_ = (enemy_.getFigure() == Figure::Cross) ? Move::Enemy : Move::Player;
            gameWindow_.changeStepString(currentMove_);
        }
    }

    void processEnemyMove() {
        uint8_t move;
        int bytesRead = recv(networkManager_.getSocket(), &move, 1, MSG_DONTWAIT);

        if (bytesRead == 0) {
            handleDisconnection();
            return;
        }

        if (bytesRead == 1 && move < 9 && gameWindow_.getCells()[move].figure == Figure::Empty) {
            executeEnemyMove(move);
        }
    }

    void executeEnemyMove(uint8_t move) {
        gameWindow_.setCell(move, enemy_.getFigure());

        gameResult_ = gameWindow_.checkResult(gameWindow_, gameWindow_.getCells());
        if (gameResult_.winner != Winner::Unknown) {
            endGame();
        } else {
            currentMove_ = Move::Player;
            gameWindow_.changeStepString(currentMove_);
        }
    }

    void handleDisconnection() {
        networkManager_.disconnect();
        gameWindow_.clearCells();
        gameWindow_.setActive(false);
        startWindow_.setActive(true);
    }

    void checkReadyStatus() {
        uint8_t buffer;
        int bytesRead = recv(networkManager_.getSocket(), &buffer, 1, MSG_DONTWAIT);

        if (bytesRead == 1 && buffer == 100) {
            networkManager_.setEnemyReady(true);
        }

        if (bytesRead == 0) {
            networkManager_.disconnect();
        }

        if (networkManager_.getReadyToContinue() && networkManager_.getEnemyReady()) {
            startNewGame();
        }
    }

    void startNewGame() {
        currentMove_ = (player_.getFigure() == Figure::Cross) ? Move::Player : Move::Enemy;
        gameWindow_.clearCells();
        gameWindow_.changeStepString(currentMove_);
        gameWindow_.setActive(true);
        resultWindow_.setActive(false);
        networkManager_.setReadyToContinue(false);
        networkManager_.setEnemyReady(false);
    }

    // Rendering
    void render() {
        window_->clear(sf::Color(20, 189, 172, 255));

        if (startWindow_.isActive()) {
            renderStartWindow();
        } else if (gameWindow_.isActive()) {
            renderGameWindow();
        } else if (resultWindow_.isActive()) {
            renderResultWindow();
        }

        window_->display();

        if (gameWindow_.isActive() && resultWindow_.isActive()) {
            sleep(1);
            gameWindow_.setActive(false);
        }
    }

    void renderStartWindow() {
        sf::Vector2i mousePos = sf::Mouse::getPosition(*window_);
        startWindow_.hover(startWindow_.getAllObjects()[2], startWindow_.getStartText(), mousePos);

        for (int i = 0; i < 3; i++) {
            window_->draw(startWindow_.getAllObjects()[i]->sprite);
        }

        for (int i = 0; i < 2; i++) {
            window_->draw(*startWindow_.getAllTexts()[i]);
        }

        window_->setMouseCursor(startWindow_.getCursor());
    }

    void renderGameWindow() {
        gameWindow_.updateCells();

        for (int i = 0; i < 9; i++) {
            if (gameWindow_.getCells()[i].figure != Figure::Empty) {
                window_->draw(gameWindow_.getCells()[i].sprite);
            }
        }

        window_->draw(gameWindow_.getAllObjects()[0]->sprite);
        window_->draw(*gameWindow_.getAllTexts()[0]);

        if (resultWindow_.isActive() && gameResult_.winner != Winner::Draw) {
            window_->draw(gameWindow_.getLineSprite());
        }

        window_->setMouseCursor(gameWindow_.getCursor());
    }

    void renderResultWindow() {
        sf::Vector2i mousePos = sf::Mouse::getPosition(*window_);
        resultWindow_.hover(resultWindow_.getAllObjects()[0], resultWindow_.getBackMenuText(), mousePos);

        renderWinnerDisplay();
        updateReadyStatusText();

        window_->draw(resultWindow_.getAllObjects()[0]->sprite);
        for (int i = 0; i < 3; i++) {
            window_->draw(*resultWindow_.getAllTexts()[i]);
        }

        window_->setMouseCursor(resultWindow_.getCursor());
    }

    void renderWinnerDisplay() {
        Object** figureButtons = startWindow_.getAllObjects();

        if (gameResult_.winner != Winner::Draw) {
            int figure = (gameResult_.winner == Winner::Player) ? (static_cast<int>(player_.getFigure()) - 1) : (static_cast<int>(enemy_.getFigure()) - 1);

            figureButtons[figure]->sprite.setPosition(200, 100);
            figureButtons[figure]->sprite.setTextureRect(sf::IntRect(200 * figure, 0, 200, 200));
            window_->draw(figureButtons[figure]->sprite);
            figureButtons[figure]->sprite.setPosition(50 + 300 * figure, 180);
            resultWindow_.setBackMenuText("WINNER!");
        } else {
            figureButtons[0]->sprite.setPosition(110, 100);
            figureButtons[0]->sprite.setTextureRect(sf::IntRect(0, 0, 400, 200));
            window_->draw(figureButtons[0]->sprite);
            figureButtons[0]->sprite.setPosition(50, 180);
            resultWindow_.setBackMenuText("DRAW!");
        }
    }

    void updateReadyStatusText() {
        if (networkManager_.getReadyToContinue() || networkManager_.getEnemyReady()) {
            resultWindow_.setWaitText("1/2 players ready...");
        } else if (networkManager_.getSocket() == -1) {
            resultWindow_.setWaitText("Opponent disconnected.");
        }
    }
};

int main() {
    GameController game;
    game.run();
    return 0;
}
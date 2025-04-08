#include <iostream>
#include <cctype>
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <map>
#include <string>
#include <thread>
#include <mutex>
#include <sstream>
#include <cmath>  
#include <random>
#include <chrono>
#include <windows.h>
#include <filesystem>

class StartScreen {
private:
    sf::RenderWindow window;
    sf::Font font;
    sf::Text titleText;
    sf::RectangleShape playButton;
    sf::Text playButtonText;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    bool playClicked;

    // Add exit button members
    sf::RectangleShape exitButton;
    sf::Text exitButtonText;

public:
    StartScreen() : playClicked(false) {
        window.create(sf::VideoMode(1200, 800), "Chess - Start Menu");
        
        // Load font
        if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
            std::cerr << "Failed to load font!" << std::endl;
            exit(EXIT_FAILURE);
        }

        // Load background texture
        if (!backgroundTexture.loadFromFile("assets/start-background.jpg")) { // Your image file
            std::cerr << "Failed to load background image!" << std::endl;
            exit(EXIT_FAILURE);
        }

        // Set up background sprite
        backgroundSprite.setTexture(backgroundTexture);
        // Scale background to window size
        sf::Vector2u textureSize = backgroundTexture.getSize();
        backgroundSprite.setScale(
            static_cast<float>(window.getSize().x) / textureSize.x,
            static_cast<float>(window.getSize().y) / textureSize.y
        );


        // Set up play button
        playButton.setSize(sf::Vector2f(300, 100));
        playButton.setFillColor(sf::Color(0, 0, 0, 150)); // Semi-transparent black
        playButton.setOutlineThickness(2);
        playButton.setOutlineColor(sf::Color::White);
        playButton.setPosition(window.getSize().x/2 - 150, 400);

        // Set up play button text
        playButtonText.setFont(font);
        playButtonText.setString("Start Game");
        playButtonText.setCharacterSize(50);
        playButtonText.setFillColor(sf::Color::White);
        sf::FloatRect textBounds = playButtonText.getLocalBounds();
        playButtonText.setOrigin(textBounds.width/2, textBounds.height/2);
        playButtonText.setPosition(window.getSize().x/2, 435);

        // Set up exit button
        exitButton.setSize(sf::Vector2f(300, 100));
        exitButton.setFillColor(sf::Color(0, 0, 0, 150));
        exitButton.setOutlineThickness(2);
        exitButton.setOutlineColor(sf::Color::White);
        exitButton.setPosition(window.getSize().x/2 - 150, 550);

        // Set up exit button text
        exitButtonText.setFont(font);
        exitButtonText.setString("Exit Game");
        exitButtonText.setCharacterSize(50);
        exitButtonText.setFillColor(sf::Color::White);
        sf::FloatRect exitTextBounds = exitButtonText.getLocalBounds();
        exitButtonText.setOrigin(exitTextBounds.width/2, exitTextBounds.height/2);
        exitButtonText.setPosition(window.getSize().x/2, 585);
    }

    void run() {
        while (window.isOpen()) {
            handleEvents();
            render();
        }
    }

    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    sf::FloatRect playbuttonBounds = playButton.getGlobalBounds();
                    sf::FloatRect exitbuttonBounds = exitButton.getGlobalBounds();
                    
                    // Check if click is within play button bounds
                    if (playbuttonBounds.contains(static_cast<sf::Vector2f>(mousePos))) {
                        playClicked = true;
                        window.close();  // Close start screen
                    }

                    // Check if click is within play button bounds
                    if (exitbuttonBounds.contains(static_cast<sf::Vector2f>(mousePos))) {
                        window.close();  // Close start screen
                    }
                }
            }
        }
    }

    void render() {
        window.clear();
        window.draw(backgroundSprite);
        window.draw(titleText);
        window.draw(playButton);
        window.draw(playButtonText);
        window.draw(exitButton);
        window.draw(exitButtonText);
        window.display();
    }

    bool isPlayClicked() const { return playClicked; }
};

class ChessGUI;

class DifficultyScreen {
private:
    sf::RenderWindow window;
    sf::Font font;
    sf::Text titleText;
    sf::RectangleShape easyButton, mediumButton, hardButton, backButton;
    sf::Text easyButtonText, mediumButtonText, hardButtonText, backButtonText;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    int selectedDifficulty; // -1=Back, 2=Easy, 3=Medium, 4=Hard

public:
    DifficultyScreen() : selectedDifficulty(-1) {
        window.create(sf::VideoMode(1200, 800), "Chess - Select Difficulty");
        
        if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
            std::cerr << "Failed to load font!" << std::endl;
            exit(EXIT_FAILURE);
        }

        // Background setup
        if (!backgroundTexture.loadFromFile("assets/difficulty-background.jpg")) {
            std::cerr << "Failed to load background image!" << std::endl;
            exit(EXIT_FAILURE);
        }
        backgroundSprite.setTexture(backgroundTexture);
        backgroundSprite.setScale(
            static_cast<float>(window.getSize().x) / backgroundTexture.getSize().x,
            static_cast<float>(window.getSize().y) / backgroundTexture.getSize().y
        );

        // Title
        titleText.setFont(font);
        titleText.setString("Select Difficulty");
        titleText.setCharacterSize(60);
        titleText.setFillColor(sf::Color::White);
        titleText.setPosition(400, 100);

        // Buttons
        const float buttonWidth = 300;
        const float buttonHeight = 100;
        const float buttonSpacing = 20;

        easyButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
        easyButton.setFillColor(sf::Color(0, 0, 0, 150));
        easyButton.setPosition(450, 250);

        easyButtonText.setFont(font);
        easyButtonText.setString("Easy");
        easyButtonText.setCharacterSize(40);
        easyButtonText.setPosition(560, 280);

        mediumButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
        mediumButton.setFillColor(sf::Color(0, 0, 0, 150));
        mediumButton.setPosition(450, 250 + buttonHeight + buttonSpacing);

        mediumButtonText.setFont(font);
        mediumButtonText.setString("Medium");
        mediumButtonText.setCharacterSize(40);
        mediumButtonText.setPosition(540, 280 + buttonHeight + buttonSpacing);

        hardButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
        hardButton.setFillColor(sf::Color(0, 0, 0, 150));
        hardButton.setPosition(450, 250 + 2*(buttonHeight + buttonSpacing));

        hardButtonText.setFont(font);
        hardButtonText.setString("Hard");
        hardButtonText.setCharacterSize(40);
        hardButtonText.setPosition(560, 280 + 2*(buttonHeight + buttonSpacing));

        backButton.setSize(sf::Vector2f(200, 60));
        backButton.setFillColor(sf::Color(150, 0, 0, 200));
        backButton.setPosition(500, 600);

        backButtonText.setFont(font);
        backButtonText.setString("Back");
        backButtonText.setCharacterSize(30);
        backButtonText.setPosition(560, 615);
    }

    void run() {
        while (window.isOpen()) {
            handleEvents();
            render();
        }
    }

    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                
                if (easyButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    selectedDifficulty = 2;
                    window.close();
                }
                else if (mediumButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    selectedDifficulty = 3;
                    window.close();
                }
                else if (hardButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    selectedDifficulty = 4;
                    window.close();
                }
                else if (backButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    selectedDifficulty = -1;
                    window.close();
                }
            }
        }
    }

    void render() {
        window.clear();
        window.draw(backgroundSprite);
        window.draw(titleText);
        window.draw(easyButton);
        window.draw(easyButtonText);
        window.draw(mediumButton);
        window.draw(mediumButtonText);
        window.draw(hardButton);
        window.draw(hardButtonText);
        window.draw(backButton);
        window.draw(backButtonText);
        window.display();
    }

    int getSelectedDifficulty() const { return selectedDifficulty; }
};

class Piece{

    public:
        enum Type{EMPTY, ROOK, KNIGHT, BISHOP, QUEEN, KING, PAWN};
        enum Color{WHITE, BLACK};
        
        Type type;
        Color color;

        Piece(Type t = EMPTY, Color c = WHITE){
            type = t;
            color = c;
        }    

        char getSymbol() const {
            if (type == EMPTY) return '.';
            char symbol;
            switch (type) {
                case PAWN:   symbol = 'P'; break;
                case KNIGHT: symbol = 'N'; break;
                case BISHOP: symbol = 'B'; break;
                case ROOK:   symbol = 'R'; break;
                case QUEEN:  symbol = 'Q'; break;
                case KING:   symbol = 'K'; break;
                default:     symbol = '.';
            }
            return (color == WHITE) ? symbol : tolower(symbol);
        }
};

class LastMove{
    public:
        int fromX, fromY;
        int toX, toY;
        Piece piece;
        bool isTwoSquarePawnMove;
        LastMove() : fromX(-1), fromY(-1), toX(-1), toY(-1), piece(), isTwoSquarePawnMove(false) {}
};

class StockfishInterface {
private:
    PROCESS_INFORMATION processInfo;
    HANDLE hChildStd_IN_Rd;
    HANDLE hChildStd_IN_Wr;
    HANDLE hChildStd_OUT_Rd;
    HANDLE hChildStd_OUT_Wr;
    bool isInitialized;

public:
    StockfishInterface() : isInitialized(false) {}

    ~StockfishInterface() {
        if (isInitialized) {
            close();
        }
    }

    bool initialize(const std::string& enginePath) {
        if (!std::filesystem::exists(enginePath)) {
            std::cerr << "Stockfish executable not found at: " << enginePath << std::endl;
            return false;
        }
        SECURITY_ATTRIBUTES saAttr;
        saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
        saAttr.bInheritHandle = TRUE;
        saAttr.lpSecurityDescriptor = NULL;

        // Create pipes for communication
        if (!CreatePipe(&hChildStd_OUT_Rd, &hChildStd_OUT_Wr, &saAttr, 0)) return false;
        if (!SetHandleInformation(hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0)) return false;

        if (!CreatePipe(&hChildStd_IN_Rd, &hChildStd_IN_Wr, &saAttr, 0)) return false;
        if (!SetHandleInformation(hChildStd_IN_Wr, HANDLE_FLAG_INHERIT, 0)) return false;

        // Set up process startup info
        STARTUPINFOA startupInfo;
        ZeroMemory(&startupInfo, sizeof(startupInfo));
        startupInfo.cb = sizeof(startupInfo);
        startupInfo.hStdError = hChildStd_OUT_Wr;
        startupInfo.hStdOutput = hChildStd_OUT_Wr;
        startupInfo.hStdInput = hChildStd_IN_Rd;
        startupInfo.dwFlags |= STARTF_USESTDHANDLES;

        // Create the process
        if (!CreateProcessA(
            NULL, 
            const_cast<char*>(enginePath.c_str()),
            NULL, NULL, TRUE, 0, NULL, NULL,
            &startupInfo, &processInfo)) {
            return false;
        }

        isInitialized = true;
        
        // Send initial commands to Stockfish
        sendCommand("uci");
        sendCommand("isready");
        
        return true;
    }

    std::string getBestMove(const std::string& fen, int moveTimeMs) {
        sendCommand("position fen " + fen);
        sendCommand("go movetime " + std::to_string(moveTimeMs));
        
        auto start = std::chrono::steady_clock::now();
        while (true) {
            std::string output = readOutput();
            size_t pos = output.find("bestmove");
            if (pos != std::string::npos) {
                size_t endPos = output.find(' ', pos + 9);
                return output.substr(pos + 9, endPos - (pos + 9)); // Extract full move (e.g., "e7e8q")
            }
            
            // Timeout check - more strict timeout
            if (std::chrono::steady_clock::now() - start > 
                std::chrono::milliseconds(moveTimeMs + 100)) {
                return ""; // Return empty string on timeout
            }
            
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        return "";
    }

    void close() {
        if (!isInitialized) return;
        
        sendCommand("quit");
        
        CloseHandle(processInfo.hProcess);
        CloseHandle(processInfo.hThread);
        CloseHandle(hChildStd_IN_Rd);
        CloseHandle(hChildStd_IN_Wr);
        CloseHandle(hChildStd_OUT_Rd);
        CloseHandle(hChildStd_OUT_Wr);
        
        isInitialized = false;
    }

    void sendCommand(const std::string& command) {
        if (!isInitialized) return;
        
        std::string cmd = command + "\n";
        DWORD bytesWritten;
        WriteFile(hChildStd_IN_Wr, cmd.c_str(), static_cast<DWORD>(cmd.size()), &bytesWritten, NULL);
    }

    std::string readOutput() {
        if (!isInitialized) return "";
        
        DWORD bytesAvailable, bytesRead;
        CHAR buffer[4096];
        std::string result;
        
        PeekNamedPipe(hChildStd_OUT_Rd, NULL, 0, NULL, &bytesAvailable, NULL);
        if (bytesAvailable > 0) {
            ReadFile(hChildStd_OUT_Rd, buffer, sizeof(buffer) - 1, &bytesRead, NULL);
            buffer[bytesRead] = '\0';
            result = buffer;
        }
        
        return result;
    }
};

class Board {
public:
    Piece squares[8][8]; // 8x8 grid of pieces
    LastMove lastmove; 
    bool hasKingMoved[2]; // Track if kings have moved (index 0 for white, 1 for black)
    bool hasRookMoved[2][2]; // Track if rooks have moved (index 0 for white, 1 for black; 0 for queenside, 1 for kingside)
    // bool isWhitecastle = false;
    // bool isBlackcastle = false;
    int kingX[2]; // Track king positions (index 0 for white, 1 for black)
    int kingY[2]; // Track king positions (index 0 for white, 1 for black)
    Piece::Color currentTurn; // Track whose turn it is
    bool isCaptureMove(int fromX, int fromY, int toX, int toY) const;
    bool canAttack(int fromX, int fromY, int toX, int toY) const;
    // bool isCastleValid(int fromX, int fromY, int toX, Piece::Color color) const;
    void performCastle(int toX, Piece::Color color);


    ChessGUI* gui;

    // Track captured pieces
    std::vector<Piece> capturedPieces[2]; // [0] for white, [1] for black

    // move history
    struct Move {
        int fromX, fromY;
        int toX, toY;
        Piece piece;
        Piece capturedPiece;
        bool isCapture;
        bool isCastle;
        bool isEnPassant;
        bool isPromotion;
        Piece::Type promotionType;

        // Add state tracking
        bool prevHasKingMoved[2];
        bool prevHasRookMoved[2][2];
        int prevKingX[2];
        int prevKingY[2];
        bool postHasKingMoved[2];
        bool postHasRookMoved[2][2];
        int postKingX[2];
        int postKingY[2];


        Move() : fromX(0), fromY(0), toX(0), toY(0), piece(), capturedPiece(),
                isCapture(false), isCastle(false), isEnPassant(false),
                isPromotion(false), promotionType(Piece::EMPTY) {}
    };
    
    std::vector<Move> moveHistory;
    std::vector<Move> undoneMoves;

    bool isValidMove(int fromX, int fromY, int toX, int toY) const;  // Add const
    bool validBishopMove(int fromX, int fromY, int toX, int toY) const;
    bool validRookMove(int fromX, int fromY, int toX, int toY) const;
    bool validQueenMove(int fromX, int fromY, int toX, int toY) const;
    
    bool hasQueen(Piece::Color color) const;

public:
    // Constructor to initialize the board
    Board() : gui(nullptr) {
        initialize();

        // Initialize castling state
        hasKingMoved[0] = hasKingMoved[1] = false;
        hasRookMoved[0][0] = hasRookMoved[0][1] = false; // White rooks
        hasRookMoved[1][0] = hasRookMoved[1][1] = false; // Black rooks
        // Initialize king positions
        kingX[0] = 4; kingY[0] = 0; // White king starts at e1
        kingX[1] = 4; kingY[1] = 7; // Black king starts at e8
        currentTurn = Piece::WHITE; // White starts first
    }

    // Initialize the board with starting positions
    void initialize() {
        // Set up white pieces
        squares[0][0] = Piece(Piece::Type::ROOK, Piece::Color::WHITE);
        squares[0][1] = Piece(Piece::Type::KNIGHT, Piece::Color::WHITE);
        squares[0][2] = Piece(Piece::Type::BISHOP, Piece::Color::WHITE);
        squares[0][3] = Piece(Piece::Type::QUEEN, Piece::Color::WHITE);
        squares[0][4] = Piece(Piece::Type::KING, Piece::Color::WHITE);
        squares[0][5] = Piece(Piece::Type::BISHOP, Piece::Color::WHITE);
        squares[0][6] = Piece(Piece::Type::KNIGHT, Piece::Color::WHITE);
        squares[0][7] = Piece(Piece::Type::ROOK, Piece::Color::WHITE);
        for (int x = 0; x < 8; x++) {
            squares[1][x] = Piece(Piece::Type::PAWN, Piece::Color::WHITE);
        }
        // Set up black pieces
        squares[7][0] = Piece(Piece::Type::ROOK, Piece::Color::BLACK);
        squares[7][1] = Piece(Piece::Type::KNIGHT, Piece::Color::BLACK);
        squares[7][2] = Piece(Piece::Type::BISHOP, Piece::Color::BLACK);
        squares[7][3] = Piece(Piece::Type::QUEEN, Piece::Color::BLACK);
        squares[7][4] = Piece(Piece::Type::KING, Piece::Color::BLACK);
        squares[7][5] = Piece(Piece::Type::BISHOP, Piece::Color::BLACK);
        squares[7][6] = Piece(Piece::Type::KNIGHT, Piece::Color::BLACK);
        squares[7][7] = Piece(Piece::Type::ROOK, Piece::Color::BLACK);
        for (int x = 0; x < 8; x++) {
            squares[6][x] = Piece(Piece::Type::PAWN, Piece::Color::BLACK);
        }

        // Empty squares
        for (int y = 2; y < 6; y++) {
            for (int x = 0; x < 8; x++) {
                squares[y][x] = Piece(); // Default constructor creates an empty piece
            }
        }
    }

    void print() const {
        std::cout << "  a b c d e f g h" << std::endl;
        for (int y = 0; y < 8; y++) {
            std::cout << y << " ";
            for (int x = 0; x < 8; x++) {
                std::cout << squares[y][x].getSymbol() << " ";
            }
            std::cout << y << std::endl;
        }
        std::cout << "  a b c d e f g h" << std::endl;
    }

    // Get a piece at a specific position
    Piece getPiece(int x, int y) const {
        return squares[y][x];
    }

    // Update last move after every move
    void updateLastMove(int fromX, int fromY, int toX, int toY, Piece piece) {
        lastmove.fromX = fromX;
        lastmove.fromY = fromY;
        lastmove.toX = toX;
        lastmove.toY = toY;
        lastmove.piece = piece;
        lastmove.isTwoSquarePawnMove = (piece.type == Piece::PAWN && abs(toY - fromY) == 2);
    }

    bool undoPair() {
        if (moveHistory.size() < 2) return false;
        
        // Undo AI move
        undoMove();
        
        // Undo player move
        bool success = undoMove();
        
        return success;
    }

    bool undoMove() {
        if (moveHistory.empty()) return false;
    
        Move lastMove = moveHistory.back();
        moveHistory.pop_back();
    
        // Restore board state
        squares[lastMove.fromY][lastMove.fromX] = lastMove.piece;
        squares[lastMove.toY][lastMove.toX] = lastMove.capturedPiece;
    
        // Restore castling state
        hasKingMoved[0] = lastMove.prevHasKingMoved[0];
        hasKingMoved[1] = lastMove.prevHasKingMoved[1];
        
        for(int i = 0; i < 2; ++i) {
            for(int j = 0; j < 2; ++j) {
                hasRookMoved[i][j] = lastMove.prevHasRookMoved[i][j];
            }
        }
        
        kingX[0] = lastMove.prevKingX[0];
        kingX[1] = lastMove.prevKingX[1];
        kingY[0] = lastMove.prevKingY[0];
        kingY[1] = lastMove.prevKingY[1];
    
        // Handle special moves
        if (lastMove.isCastle) {
            // Move rook back
            int rookFromX = (lastMove.toX > lastMove.fromX) ? 7 : 0;
            int rookToX = (lastMove.toX > lastMove.fromX) ? 5 : 3;
            int y = lastMove.fromY;
            std::swap(squares[y][rookFromX], squares[y][rookToX]);
        }
    
        if (lastMove.isEnPassant) {
            int pawnY = lastMove.piece.color == Piece::WHITE ? lastMove.toY - 1 : lastMove.toY + 1;
            squares[pawnY][lastMove.toX] = lastMove.capturedPiece;
        }
    
        if (lastMove.isPromotion) {
            squares[lastMove.fromY][lastMove.fromX].type = Piece::PAWN;
        }
    
        currentTurn = lastMove.piece.color; // Restore turn to original player
        undoneMoves.push_back(lastMove);
        return true;
    }

    bool redoMove() {
        if (undoneMoves.empty()) return false;
    
        Move nextMove = undoneMoves.back();
        undoneMoves.pop_back();
    
        // Reapply the move
        squares[nextMove.toY][nextMove.toX] = nextMove.piece;
        squares[nextMove.fromY][nextMove.fromX] = Piece();
    
        // Restore post-move state
        hasKingMoved[0] = nextMove.postHasKingMoved[0];
        hasKingMoved[1] = nextMove.postHasKingMoved[1];
        
        for(int i = 0; i < 2; ++i) {
            for(int j = 0; j < 2; ++j) {
                hasRookMoved[i][j] = nextMove.postHasRookMoved[i][j];
            }
        }
        
        kingX[0] = nextMove.postKingX[0];
        kingX[1] = nextMove.postKingX[1];
        kingY[0] = nextMove.postKingY[0];
        kingY[1] = nextMove.postKingY[1];
    
        currentTurn = (currentTurn == Piece::WHITE) ? Piece::BLACK : Piece::WHITE;
        moveHistory.push_back(nextMove);
        return true;
    }

    // Check for en passant during pawn capture
    bool isEnPassantCaptureValid(int fromX, int fromY, int toX, int toY, Piece fromPiece) const{
        if (fromPiece.type == Piece::PAWN && abs(toX - fromX) == 1 && abs(toY - fromY) == 1) {
            if (lastmove.isTwoSquarePawnMove &&
                lastmove.piece.type == Piece::PAWN &&
                lastmove.toX == toX && // Opponent's pawn is in the same file
                abs(lastmove.toY - toY) == 1) { // Your pawn is capturing diagonally
                return true; // En passant capture is valid
            }
        }
        return false;
    }

    void performEnPassantCapture(int toX, int toY) {
        // Remove the opponent's pawn from the square behind the target square
        int capturedPawnY = (lastmove.piece.color == Piece::WHITE) ? toY - 1 : toY + 1;
        squares[capturedPawnY][toX] = Piece(); // Set to empty piece
    }

    bool isCastleValid(int toX, Piece::Color color) const {
        // White castling
        if (color == Piece::WHITE) {
            if (hasKingMoved[0]) return false; // King has moved
            
            // Kingside (O-O)
            if (toX == 6 && !hasRookMoved[0][1] && 
                squares[0][5].type == Piece::EMPTY && 
                squares[0][6].type == Piece::EMPTY) {
                return true;
            }
            // Queenside (O-O-O)
            else if (toX == 2 && !hasRookMoved[0][0] && 
                     squares[0][3].type == Piece::EMPTY && 
                     squares[0][2].type == Piece::EMPTY && 
                     squares[0][1].type == Piece::EMPTY) {
                return true;
            }
        }
        // Black castling
        else {
            if (hasKingMoved[1]) return false; // King has moved
            
            // Kingside (O-O)
            if (toX == 6 && !hasRookMoved[1][1] && 
                squares[7][5].type == Piece::EMPTY && 
                squares[7][6].type == Piece::EMPTY) {
                return true;
            }
            // Queenside (O-O-O)
            else if (toX == 2 && !hasRookMoved[1][0] && 
                     squares[7][3].type == Piece::EMPTY && 
                     squares[7][2].type == Piece::EMPTY && 
                     squares[7][1].type == Piece::EMPTY) {
                return true;
            }
        }
        return false;
    }

    bool isKingTooClose(int x, int y, Piece::Color color)const{
        int opponentColorIndex = (color == Piece::WHITE) ? 1 : 0;
        int opponentKingX = kingX[opponentColorIndex];
        int opponentKingY = kingY[opponentColorIndex];
    
        // Check if (x, y) is adjacent to the opponent's king
        return (abs(x - opponentKingX) <= 1 && abs(y - opponentKingY) <= 1);
    }

    // Check for bishop attacks
    bool isBishopAttacking(int x, int y, Piece::Color opponentColor) const{
        int directions[4][2] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}}; // Diagonal directions
    
        for (int i = 0; i < 4; i++) {
            int nx = x + directions[i][0];
            int ny = y + directions[i][1];
    
            while (nx >= 0 && nx < 8 && ny >= 0 && ny < 8) {
                Piece piece = squares[ny][nx];
                if (piece.type != Piece::EMPTY) {
                    if (piece.color == opponentColor && (piece.type == Piece::BISHOP || piece.type == Piece::QUEEN)) {
                        return true; // Bishop or queen is attacking
                    }
                    break; // Stop if we encounter any piece
                }
                nx += directions[i][0];
                ny += directions[i][1];
            }
        }
    
        return false; // No bishop or queen is attacking
    }

    // Check for rook attack
    bool isRookAttacking(int x, int y, Piece::Color opponentColor) const{
        int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // Straight directions
    
        for (int i = 0; i < 4; i++) {
            int nx = x + directions[i][0];
            int ny = y + directions[i][1];
    
            while (nx >= 0 && nx < 8 && ny >= 0 && ny < 8) {
                Piece piece = squares[ny][nx];
                if (piece.type != Piece::EMPTY) {
                    if (piece.color == opponentColor && (piece.type == Piece::ROOK || piece.type == Piece::QUEEN)) {
                        return true; // Rook or queen is attacking
                    }
                    break; // Stop if we encounter any piece
                }
                nx += directions[i][0];
                ny += directions[i][1];
            }
        }
    
        return false; // No rook or queen is attacking
    }

    // Check for queen attacks
    bool isQueenAttacking(int x, int y, Piece::Color opponentColor) const{
        // Queens attack like bishops and rooks
        return isBishopAttacking(x, y, opponentColor) || isRookAttacking(x, y, opponentColor);
    }

    // Check if the King is Under Attack
    bool isSquareUnderAttack(int x, int y, Piece::Color color)const{
        // Check for attacks by pawns, knights, bishops, rooks, queens, and kings
        // Iterate through all opponent's pieces and check if they can attack (x, y)

        Piece::Color opponentColor = (color == Piece::WHITE) ? Piece::BLACK : Piece::WHITE;

        // Check if the square is adjacent to the opponent's king
        if (isKingTooClose(x, y, color)) {
            return true;
        }

        // Check for pawn attacks (with bounds checking)
        int pawnDir = (color == Piece::WHITE) ? 1 : -1;
        int attackY = y + pawnDir;
        if (attackY >= 0 && attackY < 8) { // Prevent out-of-bounds
            if (x > 0 && squares[attackY][x - 1].type == Piece::PAWN && 
                squares[attackY][x - 1].color == opponentColor) {
                return true;
            }
            if (x < 7 && squares[attackY][x + 1].type == Piece::PAWN && 
                squares[attackY][x + 1].color == opponentColor) {
                return true;
            }
        }

        // Check for knight attacks
        int knightMoves[8][2] = {{-2, -1}, {-1, -2}, {1, -2}, {2, -1}, {2, 1}, {1, 2}, {-1, 2}, {-2, 1}};
        for (int i = 0; i < 8; i++) {
            int nx = x + knightMoves[i][0];
            int ny = y + knightMoves[i][1];
            if (nx >= 0 && nx < 8 && ny >= 0 && ny < 8 &&
                squares[ny][nx].type == Piece::KNIGHT && squares[ny][nx].color == opponentColor) {
                return true;
            }
        }

         // Check for bishop attacks
        if (isBishopAttacking(x, y, opponentColor)) {
            return true;
        }

        // Check for rook attacks
        if (isRookAttacking(x, y, opponentColor)) {
            return true;
        }

        // Check for queen attacks
        if (isQueenAttacking(x, y, opponentColor)) {
            return true;
        }
        
        // Check for king attacks
        int kingMoves[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1},
        {0, 1}, {1, -1}, {1, 0}, {1, 1}};
        for (int i = 0; i < 8; i++) {
            int nx = x + kingMoves[i][0];
            int ny = y + kingMoves[i][1];
            if (nx >= 0 && nx < 8 && ny >= 0 && ny < 8 &&
                squares[ny][nx].type == Piece::KING && 
                squares[ny][nx].color == opponentColor) {
                return true;
            }
        }
        return false; // Square is not under attack
    }

    // Check if the king is in check
    bool isKingInCheck(Piece::Color color) const{
        int colorIndex = (color == Piece::WHITE) ? 0 : 1;
        return isSquareUnderAttack(kingX[colorIndex], kingY[colorIndex], color);
    }

    //  Check for Checkmate
    bool isCheckmate(Piece::Color color) {
        if (!isKingInCheck(color)) {
            return false; // Not in check, so not checkmate
        }
    
        // Check if any move can get the king out of check
        for (int y = 0; y < 8; y++) {
            for (int x = 0; x < 8; x++) {
                Piece piece = squares[y][x];
                if (piece.type != Piece::EMPTY && piece.color == color) {
                    for (int ty = 0; ty < 8; ty++) {
                        for (int tx = 0; tx < 8; tx++) {
                            if (isValidMove(x, y, tx, ty)) {
                                // Simulate the move
                                Piece tempTo = squares[ty][tx];
                                Piece tempFrom = squares[y][x];
                                squares[ty][tx] = piece;
                                squares[y][x] = Piece();
    
                                // Update king position if moving the king
                                int oldKingX = kingX[color == Piece::WHITE ? 0 : 1];
                                int oldKingY = kingY[color == Piece::WHITE ? 0 : 1];
                                if (piece.type == Piece::KING) {
                                    kingX[color == Piece::WHITE ? 0 : 1] = tx;
                                    kingY[color == Piece::WHITE ? 0 : 1] = ty;
                                }
    
                                // Check if the king is still in check after the move
                                bool stillInCheck = (piece.type == Piece::KING) 
                                                    ? isSquareUnderAttack(tx, ty, color)
                                                    : isKingInCheck(color);
    
                                // Undo the move and restore king position
                                squares[y][x] = tempFrom;
                                squares[ty][tx] = tempTo;
                                if (piece.type == Piece::KING) {
                                    kingX[color == Piece::WHITE ? 0 : 1] = oldKingX;
                                    kingY[color == Piece::WHITE ? 0 : 1] = oldKingY;
                                }
    
                                if (!stillInCheck) {
                                    return false; // Found a valid escape
                                }
                            }
                        }
                    }
                }
            }
        }
    
        return true; // No moves to escape check
    }

    // Cheeck for Stalemate
    bool isStalemate(Piece::Color color) {
        if (isKingInCheck(color)) {
            return false; // Not stalemate if the king is in check
        }
    
        // Check if any legal move is available
        for (int y = 0; y < 8; y++) {
            for (int x = 0; x < 8; x++) {
                Piece piece = squares[y][x];
                if (piece.type != Piece::EMPTY && piece.color == color) {
                    for (int ty = 0; ty < 8; ty++) {
                        for (int tx = 0; tx < 8; tx++) {
                            if (isValidMove(x, y, tx, ty)) {
                                return false; // Found a legal move
                            }
                        }
                    }
                }
            }
        }
    
        return true; // No legal moves available
    }

    
    const std::vector<Move>& getMoveHistory() const { 
        return moveHistory; 
    }

    // Member function declarations
    void setGUI(ChessGUI* guiRef);

    // Function to execute a move and update the board
    bool executeMove(int fromX, int fromY, int toX, int toY);
    
};

// Replace ChessAI with this minimal Stockfish wrapper
class StockfishWrapper {
private:
    StockfishInterface engine;
    std::string enginePath;
    
public:
    StockfishWrapper(const std::string& path) : enginePath(path) {}
    
    bool connect() {
        return engine.initialize(enginePath);
    }
    
    std::string getBestMove(const std::string& fen, int thinkTimeMs) {
        return engine.getBestMove(fen, thinkTimeMs);
    }

    void sendCommand(const std::string& command) {
        engine.sendCommand(command);  // Forward to the engine's implementation
    }
};

class ChessGUI {
    private:
        sf::RenderWindow window;
        sf::Texture boardTexture;
        sf::Sprite boardSprite;
        std::map<std::string, sf::Texture> pieceTextures;
        Board board;
        bool isWhiteTurn = true;
        sf::Font font;
        sf::Text turnText;
        sf::Text result;
        int MoveCount = 0;
        bool vsAI = true;
        StockfishWrapper stockfish;
        Piece::Color aiColor = Piece::BLACK; // AI plays as black
        bool aiThinking = false;
        
        void render();
        void drawCapturedPieces();
        void drawMoveHistory();
        void drawSidePanel();
    
        // movehistory
        float moveHistoryScrollOffset = 0;
        bool isScrolling = false;
        float lastMouseY = 0;
        float totalMoveHeight = 0;
    
        const float lineHeight = 25.0f;
        const float panelX = 1200.0f;
        const float panelY = 100.0f;
        const float panelWidth = 400.0f;
        const float panelHeight = 600.0f;
        const float scrollbarWidth = 15.0f;
    
        const float moveHistoryX = 1220.0f;
        const float moveHistoryY = 100.0f;
    
        // undo and redo
        sf::RectangleShape undoButton, redoButton;
        sf::Text undoButtonText, redoButtonText;
    
        std::mutex boardMutex;
        
        // Dragging state
        int selectedX = -1;
        int selectedY = -1;
        bool isDragging = false;
        sf::Sprite draggedPiece;
        sf::Vector2f dragOffset;
    
        float dragPieceWidth;
        float dragPieceHeight;
    
        // Promotion
        bool isPromoting = false;
        int promotionX = -1;
        int promotionY = -1;
        Piece::Color promotionColor;
    
        // restart
        bool restartRequested = false;
    
        // Game result dialog
        bool gameOver = false;
        std::string gameResultMessage;
        sf::RectangleShape gameOverlay;
        sf::RectangleShape gameResultDialog;
        sf::Text gameResultText;
        sf::RectangleShape newGameButton;
        sf::Text newGameButtonText;
        sf::RectangleShape exitGameButton;
        sf::Text exitGameButtonText;
    
        // resign button
        sf::RectangleShape resignButton;
        sf::Text resignButtonText;
        
        // Resign dialog
        bool isResignDialogOpen = false;
        sf::RectangleShape resignDialog;
        sf::Text resignDialogText;
        sf::RectangleShape resignDialogYesButton;
        sf::Text resignDialogYesText;
        sf::RectangleShape resignDialogNoButton;
        sf::Text resignDialogNoText;
    
        // exit button
        sf::RectangleShape exitButton;
        sf::Text exitButtonText;
    
        // move to algebraic
        std::string moveToAlgebraic(const Board::Move& move);
        
        // Piece highlight
        sf::Color highlightColor = sf::Color(255, 255, 0, 100);
    
        // draw valid square
        void drawValidMoves(); 
    
        void drawHighlight() {
            if (selectedX == -1 || selectedY == -1) return;
        
            sf::RectangleShape highlight(sf::Vector2f(100, 100));
            highlight.setFillColor(highlightColor);
            highlight.setPosition(selectedX * 100, (7 - selectedY) * 100);
            window.draw(highlight);
        }
    
        // Helper function to convert Piece::Color to string
        std::string colorToString(Piece::Color color) {
            return (color == Piece::WHITE) ? "White" : "Black";
        }
    
        void drawCheckIndicator() {
            if (!board.isKingInCheck(board.currentTurn)) return;
    
            int colorIndex = (board.currentTurn == Piece::WHITE) ? 0 : 1;
            int kingX = board.kingX[colorIndex];
            int kingY = board.kingY[colorIndex];
    
            sf::RectangleShape indicator(sf::Vector2f(100, 100));
            indicator.setFillColor(sf::Color(255, 0, 0, 80));
            indicator.setPosition(kingX * 100, (7 - kingY) * 100);
            
            float thickness = 3.0f + std::sin(sf::seconds(1.0f).asSeconds() * 5.0f);
            indicator.setOutlineThickness(thickness);
            indicator.setOutlineColor(sf::Color::Red);
            
            window.draw(indicator);
        }
    
    public: 
        struct ValidMove {
            int x, y;
            bool isCapture;
            ValidMove(int x_, int y_, bool capture) : x(x_), y(y_), isCapture(capture) {}      
        };
        std::vector<ValidMove> validMoves;
    
    public:
        ChessGUI(int difficulty) : window(sf::VideoMode(1600, 800), "Chess"), stockfish("stockfish-windows-x86-64-avx2.exe") {
            window.setActive(false);
            loadTextures();
            initializeBoard();
    
            board.setGUI(this);
    
            if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
                std::cerr << "Failed to load font!" << std::endl;
                exit(EXIT_FAILURE);
            }
    
            if (!stockfish.connect()) {
                std::cerr << "Failed to initialize Stockfish!" << std::endl;
                exit(1);
            }
    
            // Set difficulty (0-20 scale)
            stockfish.sendCommand("setoption name Skill Level value " + std::to_string(difficulty * 4));
    
            // Set up turn display text
            turnText.setFont(font);
            turnText.setCharacterSize(24);
            turnText.setFillColor(sf::Color::Black);
            turnText.setPosition(930, 300);
            updateTurnText();
    
            // Set up Result display text
            result.setFont(font);
            result.setCharacterSize(20);
            result.setFillColor(sf::Color::Black);
            result.setPosition(942, 400);
            updateResultText();
    
            // Game result dialog
            gameOverlay.setSize(sf::Vector2f(1200, 800));
            gameOverlay.setFillColor(sf::Color(0, 0, 0, 150));
    
            gameResultDialog.setSize(sf::Vector2f(500, 200));
            gameResultDialog.setFillColor(sf::Color(240, 240, 240));
            gameResultDialog.setOutlineThickness(2);
            gameResultDialog.setOutlineColor(sf::Color::Black);
            gameResultDialog.setPosition(200, 300);
    
            gameResultText.setFont(font);
            gameResultText.setCharacterSize(30);
            gameResultText.setFillColor(sf::Color::Black);
            gameResultText.setPosition(450, 350);
    
            newGameButton.setSize(sf::Vector2f(140, 50));
            newGameButton.setFillColor(sf::Color(0, 150, 0));
            newGameButton.setPosition(290, 400);
    
            newGameButtonText.setFont(font);
            newGameButtonText.setString("New Game");
            newGameButtonText.setCharacterSize(20);
            newGameButtonText.setFillColor(sf::Color::White);
            newGameButtonText.setPosition(310, 410);
    
            exitGameButton.setSize(sf::Vector2f(140, 50));
            exitGameButton.setFillColor(sf::Color(150, 0, 0));
            exitGameButton.setPosition(470, 400);
    
            exitGameButtonText.setFont(font);
            exitGameButtonText.setString("Exit");
            exitGameButtonText.setCharacterSize(20);
            exitGameButtonText.setFillColor(sf::Color::White);
            exitGameButtonText.setPosition(520, 410);
    
            // Resign button
            resignButton.setSize(sf::Vector2f(100, 50));
            resignButton.setFillColor(sf::Color(150, 0, 0));
            resignButton.setPosition(960, 730);
    
            resignButtonText.setFont(font);
            resignButtonText.setString("Resign");
            resignButtonText.setCharacterSize(20);
            resignButtonText.setFillColor(sf::Color::White);
            resignButtonText.setPosition(980, 740);
    
            // Resign dialog
            resignDialog.setSize(sf::Vector2f(400, 200));
            resignDialog.setFillColor(sf::Color(210, 210, 210));
            resignDialog.setOutlineThickness(2);
            resignDialog.setOutlineColor(sf::Color::Black);
            resignDialog.setPosition(200, 300);
    
            resignDialogText.setFont(font);
            resignDialogText.setString("Are you sure you want to resign?");
            resignDialogText.setCharacterSize(22);
            resignDialogText.setFillColor(sf::Color::Black);
            resignDialogText.setPosition(240, 350);
    
            resignDialogYesButton.setSize(sf::Vector2f(80, 40));
            resignDialogYesButton.setFillColor(sf::Color(0, 150, 0));
            resignDialogYesButton.setPosition(300, 400);
            
            resignDialogYesText.setFont(font);
            resignDialogYesText.setString("Yes");
            resignDialogYesText.setCharacterSize(24);
            resignDialogYesText.setFillColor(sf::Color::White);
            resignDialogYesText.setPosition(320, 405);
    
            resignDialogNoButton.setSize(sf::Vector2f(80, 40));
            resignDialogNoButton.setFillColor(sf::Color(150, 0, 0));
            resignDialogNoButton.setPosition(420, 400);
            
            resignDialogNoText.setFont(font);
            resignDialogNoText.setString("No");
            resignDialogNoText.setCharacterSize(24);
            resignDialogNoText.setFillColor(sf::Color::White);
            resignDialogNoText.setPosition(440, 405);
    
            // Set up exit button
            exitButton.setSize(sf::Vector2f(100, 50));
            exitButton.setFillColor(sf::Color(0, 0, 0));
            exitButton.setPosition(1080, 730);
    
            exitButtonText.setFont(font);
            exitButtonText.setString("Exit");
            exitButtonText.setCharacterSize(20);
            exitButtonText.setFillColor(sf::Color::White);
            exitButtonText.setPosition(1110, 740);
    
            // Undo Button
            undoButton.setSize(sf::Vector2f(80, 40));
            undoButton.setFillColor(sf::Color(50, 150, 50));
            undoButton.setPosition(1010, 50);
    
            undoButtonText.setFont(font);
            undoButtonText.setString("Undo");
            undoButtonText.setCharacterSize(20);
            undoButtonText.setPosition(1025, 57);
    
            // Redo Button
            redoButton.setSize(sf::Vector2f(80, 40));
            redoButton.setFillColor(sf::Color(50, 50, 150));
            redoButton.setPosition(1110, 50);
    
            redoButtonText.setFont(font);
            redoButtonText.setString("Redo");
            redoButtonText.setCharacterSize(20);
            redoButtonText.setPosition(1125, 57);
    
            window.setActive(true);
        }
    
        void updateTurnText() {
            turnText.setString((board.currentTurn == Piece::WHITE) ? "White's Turn" : "Black's Turn");
        }
    
        void updateResultText() {
            if(board.isCheckmate(Piece::WHITE)) {
                gameResultMessage = "Black Wins by Checkmate!";
                gameOver = true;
            }
            else if(board.isCheckmate(Piece::BLACK)) {
                gameResultMessage = "White Wins by Checkmate!";
                gameOver = true;
            }
            else if(board.isStalemate(board.currentTurn)) {
                gameResultMessage = "Match Draw!";
                gameOver = true;
            }
            else {
                gameResultMessage = "Game in Progress";
                gameOver = false;
            }
        
            gameResultText.setString(gameResultMessage);
            sf::FloatRect textRect = gameResultText.getLocalBounds();
            gameResultText.setOrigin(textRect.left + textRect.width/2.0f,
                                   textRect.top + textRect.height/2.0f);
        }
    
        void resetGame() {
            board.initialize();
            validMoves.clear();
        
            board.currentTurn = Piece::WHITE;
            MoveCount = 0;
            gameOver = false;
            isPromoting = false;
            aiThinking = false;
            
            board.capturedPieces[0].clear();
            board.capturedPieces[1].clear();
        
            board.moveHistory.clear();  
            
            moveHistoryScrollOffset = 0;  
            
            updateTurnText();
            updateResultText();
        }
    
        void run() {
            while (window.isOpen()) {
                handleInput();
                
                if (vsAI && board.currentTurn == aiColor) {
                    handleAITurn();
                }
                
                render();
                sf::sleep(sf::milliseconds(16));
            }
        }
    
        std::string boardToFEN() const {
            std::string fen;
            
            // Piece placement
            for (int y = 7; y >= 0; y--) {
                int emptyCount = 0;
                for (int x = 0; x < 8; x++) {
                    Piece p = board.getPiece(x, y);
                    if (p.type == Piece::EMPTY) {
                        emptyCount++;
                    } else {
                        if (emptyCount > 0) {
                            fen += std::to_string(emptyCount);
                            emptyCount = 0;
                        }
                        fen += p.getSymbol();
                    }
                }
                if (emptyCount > 0) fen += std::to_string(emptyCount);
                if (y > 0) fen += '/';
            }
            
            // Active color
            fen += (board.currentTurn == Piece::WHITE) ? " w " : " b ";
            
            // Castling availability
            std::string castling;
            if (!board.hasKingMoved[0]) {
                if (!board.hasRookMoved[0][1]) castling += 'K';
                if (!board.hasRookMoved[0][0]) castling += 'Q';
            }
            if (!board.hasKingMoved[1]) {
                if (!board.hasRookMoved[1][1]) castling += 'k';
                if (!board.hasRookMoved[1][0]) castling += 'q';
            }
            fen += (castling.empty()) ? "- " : castling + " ";
            
            // En passant
            if (board.lastmove.isTwoSquarePawnMove) {
                char file = 'a' + board.lastmove.toX;
                int rank = (board.lastmove.piece.color == Piece::WHITE) ? 6 : 3;
                fen += file + std::to_string(rank) + " ";
            } else {
                fen += "- ";
            }
            
            fen += "0 1";
            
            return fen;
        }
    
    private:
        void loadTextures() {
            if (!boardTexture.loadFromFile("chessboard.png")) {
                std::cerr << "Failed to load chessboard texture!" << std::endl;
                exit(EXIT_FAILURE);
            }
            boardSprite.setTexture(boardTexture);
            boardSprite.setScale(800.0f / boardTexture.getSize().x, 800.0f / boardTexture.getSize().y);
    
            std::string pieces[] = {"pawn", "knight", "bishop", "rook", "queen", "king"};
            std::string colors[] = {"W_", "B_"};
    
            for (const auto& color : colors) {
                for (const auto& piece : pieces) {
                    std::string key = color + piece;
                    sf::Texture texture;
                    if (!texture.loadFromFile("assets/" + key + ".png")) {
                        std::cerr << "Failed to load texture: " << key << std::endl;
                        exit(EXIT_FAILURE);
                    }
                    pieceTextures[key] = texture;
                }
            }
        }
    
        void initializeBoard() {
            board.initialize();
        }
    
        sf::Sprite createBoardPieceSprite(int x, int y) {
            Piece piece = board.getPiece(x, y);
            if (piece.type == Piece::EMPTY) return sf::Sprite();
    
            std::string key = (piece.color == Piece::WHITE ? "W_" : "B_");
            switch (piece.type) {
                case Piece::PAWN:   key += "pawn";   break;
                case Piece::KNIGHT: key += "knight"; break;
                case Piece::BISHOP: key += "bishop"; break;
                case Piece::ROOK:   key += "rook";   break;
                case Piece::QUEEN:  key += "queen";  break;
                case Piece::KING:   key += "king";   break;
                default: return sf::Sprite();
            }
    
            sf::Sprite sprite(pieceTextures.at(key));
            const float squareSize = 100.0f;
    
            sf::Vector2u texSize = sprite.getTexture()->getSize();
            float scale = std::min(squareSize / texSize.x, squareSize / texSize.y);
            sprite.setScale(scale, scale);
    
            sf::FloatRect bounds = sprite.getGlobalBounds();
            sprite.setPosition(
                x * squareSize + (squareSize - bounds.width) / 2,
                (7-y) * squareSize + (squareSize - bounds.height) / 2
            );
    
            return sprite;
        }
    
        void handleAITurn() {
            // Run Stockfish in a separate thread
            std::thread([this]() {
                std::string fen = boardToFEN();
                std::string move = stockfish.getBestMove(fen, 100);
                
                if (!move.empty() && move.length() >= 4) {
                    int fromX = move[0] - 'a';
                    int fromY = move[1] - '1';
                    int toX = move[2] - 'a';
                    int toY = move[3] - '1';
                    
                    // Use mutex to protect board access
                    std::lock_guard<std::mutex> lock(boardMutex);
                    if (board.executeMove(fromX, fromY, toX, toY)) {
                        // Handle promotion
                        Piece& movedPiece = board.squares[toY][toX];
                        if (movedPiece.type == Piece::PAWN && 
                            ((movedPiece.color == Piece::BLACK && toY == 0) ||
                             (movedPiece.color == Piece::WHITE && toY == 7))) {
                            movedPiece.type = Piece::QUEEN;
                        }
                        
                        MoveCount++;
                        updateTurnText();
                        updateResultText();
                    }
                }
            }).detach(); // Detach the thread to run independently
        }
    
        void handleInput() {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
    
                if (isResignDialogOpen) {
                    validMoves.clear();
                    if (event.type == sf::Event::MouseButtonPressed && 
                        event.mouseButton.button == sf::Mouse::Left) {
                        
                        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                        if (resignDialogYesButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                            window.close();
                        }
                        else if (resignDialogNoButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                            isResignDialogOpen = false;
                        }
                    }
                    continue;
                }
    
                if(gameOver) {
                    if (event.type == sf::Event::MouseButtonPressed && 
                        event.mouseButton.button == sf::Mouse::Left) {
                        
                        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                        
                        if(newGameButton.getGlobalBounds().contains(
                            static_cast<sf::Vector2f>(mousePos))) {
                            resetGame();
                            restartRequested = true;
                        }
                        else if(exitGameButton.getGlobalBounds().contains(
                            static_cast<sf::Vector2f>(mousePos))) {
                            window.close();
                        }
                    }
                    continue;
                }
    
                if (event.type == sf::Event::MouseButtonPressed && 
                    event.mouseButton.button == sf::Mouse::Left) {
                    
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    if (exitButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                        window.close();
                    }
                    else if (resignButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                        isResignDialogOpen = true;
                    }
                }
                
                if (event.type == sf::Event::MouseButtonPressed) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    
                    if (undoButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        std::lock_guard<std::mutex> lock(boardMutex);
                        if (vsAI) {
                            if (board.getMoveHistory().size() >= 2) {
                                board.undoPair();
                                validMoves.clear();
                            } else {
                                board.undoMove();
                                validMoves.clear();
                            }
                        } else {
                            board.undoMove();
                            validMoves.clear();
                        }
                        updateTurnText();
                        updateResultText();
                    }
                    else if (redoButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        std::lock_guard<std::mutex> lock(boardMutex);
                        board.redoMove();
                        validMoves.clear();
                        updateTurnText();
                        updateResultText();
                    }
                }
                
                if (isPromoting) {
                    validMoves.clear();
                    if (event.type == sf::Event::MouseButtonPressed) {
                        handlePromotionSelection(event.mouseButton.x, event.mouseButton.y);
                    }
                    continue;
                }
                else if (!vsAI || (vsAI && board.currentTurn != aiColor)) {
                    if (event.type == sf::Event::MouseButtonPressed && 
                        event.mouseButton.button == sf::Mouse::Left) {
                        
                        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                        int guiX = mousePos.x / 100;
                        int guiY = mousePos.y / 100;
                        int boardX = guiX;
                        int boardY = 7 - guiY;
        
                        if (board.getPiece(boardX, boardY).type != Piece::EMPTY &&
                            board.getPiece(boardX, boardY).color == board.currentTurn) {
                            
                            validMoves.clear();
                            
                            selectedX = boardX;
                            selectedY = boardY;
                            isDragging = true;
    
                            if (selectedX != -1 && selectedY != -1) {
                                for (int ty = 0; ty < 8; ty++) {
                                    for (int tx = 0; tx < 8; tx++) {
                                        if (board.isValidMove(selectedX, selectedY, tx, ty)) {
                                            bool isCapture = board.isCaptureMove(selectedX, selectedY, tx, ty);
                                            validMoves.emplace_back(tx, ty, isCapture);
                                        }
                                    }
                                }
                            }
        
                            draggedPiece = createBoardPieceSprite(selectedX, selectedY);
                            
                            sf::FloatRect bounds = draggedPiece.getGlobalBounds();
                            dragPieceWidth = bounds.width;
                            dragPieceHeight = bounds.height;
        
                            draggedPiece.setPosition(
                                mousePos.x - dragPieceWidth / 2,
                                mousePos.y - dragPieceHeight / 2
                            );
                        }
                    }
        
                    if (event.type == sf::Event::MouseMoved && isDragging) {
                        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                        draggedPiece.setPosition(
                            mousePos.x - dragPieceWidth / 2,
                            mousePos.y - dragPieceHeight / 2
                        );
                    }
        
                    if (event.type == sf::Event::MouseButtonReleased && 
                        event.mouseButton.button == sf::Mouse::Left && isDragging) {
                        
                        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                        int newGuiX = mousePos.x / 100;
                        int newGuiY = mousePos.y / 100;
                        int newBoardX = newGuiX;
                        int newBoardY = 7 - newGuiY;
        
                        if (board.executeMove(selectedX, selectedY, newBoardX, newBoardY)) {
                            validMoves.clear();
                            MoveCount++;
                            updateTurnText();
                            updateResultText();
        
                            Piece movedPiece = board.getPiece(newBoardX, newBoardY);
                            if (movedPiece.type == Piece::PAWN && 
                                ((movedPiece.color == Piece::WHITE && newBoardY == 7) ||
                                (movedPiece.color == Piece::BLACK && newBoardY == 0))) {
                                isPromoting = true;
                                promotionX = newBoardX;
                                promotionY = newBoardY;
                                promotionColor = movedPiece.color;
        
                                isDragging = false;
                                selectedX = -1;
                                selectedY = -1;
        
                                return;
                            }
        
                            if (vsAI && board.currentTurn == aiColor && !aiThinking) {
                                aiThinking = true;
                                handleAITurn();
                                aiThinking = false;
                            }
                        }
        
                        isDragging = false;
                        selectedX = -1;
                        selectedY = -1;
                    }
                }
        
                if(event.type == sf::Event::MouseWheelScrolled) {
                    if(event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                        moveHistoryScrollOffset -= event.mouseWheelScroll.delta * 20;
                        moveHistoryScrollOffset = std::max(0.f, std::min(
                            moveHistoryScrollOffset, 
                            totalMoveHeight - 600.f
                        ));
                    }
                }
        
                if(event.type == sf::Event::MouseButtonPressed) {
                    if(event.mouseButton.button == sf::Mouse::Left) {
                        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                        float scrollbarLeft = moveHistoryX + panelWidth - scrollbarWidth - 5;
                        float scrollbarRight = scrollbarLeft + scrollbarWidth;
                        
                        if(mousePos.x >= scrollbarLeft && mousePos.x <= scrollbarRight &&
                        mousePos.y >= moveHistoryY && mousePos.y <= moveHistoryY + panelHeight) {
                            isScrolling = true;
                            lastMouseY = mousePos.y;
                        }
                    }
                }
        
                if(event.type == sf::Event::MouseButtonReleased) {
                    if(event.mouseButton.button == sf::Mouse::Left) {
                        isScrolling = false;
                    }
                }
        
                if(event.type == sf::Event::MouseMoved && isScrolling) {
                    float deltaY = sf::Mouse::getPosition(window).y - lastMouseY;
                    lastMouseY = sf::Mouse::getPosition(window).y;
                    
                    moveHistoryScrollOffset += deltaY * (totalMoveHeight / 600.f);
                    moveHistoryScrollOffset = std::max(0.f, std::min(
                        moveHistoryScrollOffset, 
                        totalMoveHeight - 600.f
                    ));
                }
            }
        }
    
    
        // Helper function to create sprites for captured pieces
        sf::Sprite createPieceSpriteFromType(Piece::Type type, Piece::Color color) {
            std::string key = (color == Piece::WHITE ? "W_" : "B_");
            switch(type) {
                case Piece::PAWN: key += "pawn"; break;
                case Piece::KNIGHT: key += "knight"; break;
                case Piece::BISHOP: key += "bishop"; break;
                case Piece::ROOK: key += "rook"; break;
                case Piece::QUEEN: key += "queen"; break;
                default: return sf::Sprite();
            }
        
            sf::Sprite sprite(pieceTextures.at(key));
            sprite.setScale(0.4f, 0.4f); // Smaller size for captured pieces
            return sprite;
        }
    
        private:
        // Add this helper function to count captured pieces
        std::map<Piece::Type, int> countCaptured(const std::vector<Piece>& pieces) {
            std::map<Piece::Type, int> counts;
            for (const auto& piece : pieces) {
                if (piece.type != Piece::KING) { // Kings are never captured
                    counts[piece.type]++;
                }
            }
            return counts;
        }
    
        void drawPromotionMenu() {
            const int squareSize = 100;
            const int baseX = promotionX * squareSize;
            const int baseY = (7 - promotionY) * squareSize;
    
            int startX = std::max(0, std::min(baseX - 2 * squareSize, 800 - 4 * squareSize));
    
            sf::RectangleShape overlay(sf::Vector2f(800, 800));
            overlay.setFillColor(sf::Color(0, 0, 0, 150));
            window.draw(overlay);
    
            // Define promotion types in the correct order
            std::vector<Piece::Type> types = {Piece::QUEEN, Piece::ROOK, Piece::BISHOP, Piece::KNIGHT};
            
            for (int i = 0; i < 4; i++) {
                sf::Sprite sprite = createPieceSpriteFromType(types[i], promotionColor);
                sprite.setPosition(startX + i * squareSize, baseY);
                window.draw(sprite);
            }
        }
    
        void handlePromotionSelection(int mouseX, int mouseY) {
            std::lock_guard<std::mutex> lock(boardMutex);
            
            const int squareSize = 100;
            const int baseY = (7 - promotionY) * squareSize;
            const int startX = std::max(0, std::min(promotionX * squareSize - 2 * squareSize, 800 - 4 * squareSize));
    
            std::vector<Piece::Type> types = {Piece::QUEEN, Piece::ROOK, Piece::BISHOP, Piece::KNIGHT};
    
            for (int i = 0; i < 4; i++) {
                sf::FloatRect optionRect(
                    startX + i * squareSize, 
                    baseY,
                    squareSize,
                    squareSize
                );
    
                if (optionRect.contains(static_cast<float>(mouseX), static_cast<float>(mouseY))) {
                    board.squares[promotionY][promotionX].type = types[i];
                    isPromoting = false;
    
                    // Reset dragging state
                    isDragging = false;
                    selectedX = -1;
                    selectedY = -1;
    
                    board.currentTurn = (board.currentTurn == Piece::WHITE) ? Piece::BLACK : Piece::WHITE;
                    updateTurnText();
                    updateResultText();
                    break;
                }
            }
        }
    };

// Modified drawCapturedPieces function
void ChessGUI::drawCapturedPieces() {
    // Position parameters
    const float iconSize = 30.f;
    const float verticalSpacing = 10.f;
    const float horizontalSpacing = 40.f;
    const int maxPiecesPerColumn = 5;

    // White's captured pieces (black) at bottom right
    float whiteStartX = 810.f;
    float whiteStartY = 730.f;  // Bottom of the window

    // Black's captured pieces (white) at top right
    float blackStartX = 810.f;
    float blackStartY = 40.f;   // Top of the window

    // Get captured pieces from board
    const auto& whiteCaptured = board.capturedPieces[0];
    const auto& blackCaptured = board.capturedPieces[1];

    // Count captured pieces by type
    auto whiteCounts = countCaptured(whiteCaptured);
    auto blackCounts = countCaptured(blackCaptured);

    // Draw white's captured pieces (black)
    float currentX = whiteStartX;
    float currentY = whiteStartY;
    int piecesInColumn = 0;
    for (const auto& [type, count] : whiteCounts) {
        for (int i = 0; i < count; i++) {
            sf::Sprite sprite = createPieceSpriteFromType(type, Piece::BLACK);
            sprite.setScale(iconSize / 100.f, iconSize / 100.f);
            sprite.setPosition(currentX, currentY);
            window.draw(sprite);

            // Update position
            currentY -= (iconSize + verticalSpacing);
            piecesInColumn++;

            // New column after 5 pieces
            if (piecesInColumn >= maxPiecesPerColumn) {
                currentX += horizontalSpacing;
                currentY = whiteStartY;
                piecesInColumn = 0;
            }
        }
    }

    // Draw black's captured pieces (white)
    currentX = blackStartX;
    currentY = blackStartY;
    piecesInColumn = 0;
    for (const auto& [type, count] : blackCounts) {
        for (int i = 0; i < count; i++) {
            sf::Sprite sprite = createPieceSpriteFromType(type, Piece::WHITE);
            sprite.setScale(iconSize / 100.f, iconSize / 100.f);
            sprite.setPosition(currentX, currentY);
            window.draw(sprite);

            // Update position
            currentY += (iconSize + verticalSpacing);
            piecesInColumn++;

            // New column after 5 pieces
            if (piecesInColumn >= maxPiecesPerColumn) {
                currentX += horizontalSpacing;
                currentY = blackStartY;
                piecesInColumn = 0;
            }
        }
    }
}

// Move history
void ChessGUI::drawMoveHistory() {
    const auto& moves = board.getMoveHistory();
    
    // Calculate total content height
    totalMoveHeight = std::ceil(moves.size() / 2.0f) * lineHeight;
    
    // Draw scrollbar track if needed
    if (totalMoveHeight > panelHeight) {
        sf::RectangleShape scrollTrack(sf::Vector2f(scrollbarWidth, panelHeight));
        // scrollTrack.setFillColor(sf::Color(255, 255, 255));
        scrollTrack.setFillColor(sf::Color(60, 60, 60)); // Dark grey
        // scrollTrack.setPosition(
        //     moveHistoryX + panelWidth - scrollbarWidth - 5,
        //     moveHistoryY
        // );
        scrollTrack.setPosition(
            1600 - scrollbarWidth - 5,
            moveHistoryY
        );
        window.draw(scrollTrack);

        // Draw scroll thumb
        float thumbHeight = panelHeight * (panelHeight / totalMoveHeight);
        thumbHeight = std::max(20.f, thumbHeight); // Minimum thumb size
        
        float thumbPosition = (moveHistoryScrollOffset / totalMoveHeight) * panelHeight;
        
        sf::RectangleShape scrollThumb(sf::Vector2f(scrollbarWidth - 2, thumbHeight));
        scrollThumb.setFillColor(sf::Color(240, 240, 240)); // Darker gray
        // scrollThumb.setPosition(
        //     moveHistoryX + panelWidth - scrollbarWidth - 3,
        //     moveHistoryY + thumbPosition
        // );
        scrollThumb.setPosition(
            1600 - scrollbarWidth - 3,
            moveHistoryY + thumbPosition
        );
        window.draw(scrollThumb);
    }

    // Set up view for move history clipping
    sf::View originalView = window.getView();
    sf::View scrollView;
    scrollView.reset(sf::FloatRect(0, 0, panelWidth, panelHeight));
    scrollView.setViewport(sf::FloatRect(
        panelX / window.getSize().x,
        panelY / window.getSize().y,
        panelWidth / window.getSize().x,
        panelHeight / window.getSize().y
    ));
    window.setView(scrollView);

    // Draw moves within the clipped area
    sf::Text moveText("", font, 18);
    moveText.setFillColor(sf::Color::White);
    float currentY = -moveHistoryScrollOffset;

    for (size_t i = 0; i < moves.size(); i += 2) {
        // Skip moves that are above the visible area
        if (currentY + lineHeight < 0) {
            currentY += lineHeight;
            continue;
        }

        // Stop drawing if below visible area
        if (currentY > panelHeight) {
            break;
        }

        std::stringstream ss;
        int moveNumber = (i / 2) + 1;
        ss << moveNumber << ".  " << moveToAlgebraic(moves[i]);
        if (i + 1 < moves.size()) {
            ss << "   " << moveToAlgebraic(moves[i + 1]);
        }

        moveText.setString(ss.str());
        moveText.setPosition(10.0f, currentY);
        window.draw(moveText);

        currentY += lineHeight;
    }

    // Restore original view
    window.setView(originalView);
}

// Helper function for sidebar elements
void ChessGUI::drawSidePanel() {

    // Draw left sidebar background
    sf::RectangleShape leftSidebar(sf::Vector2f(400, 800));
    leftSidebar.setPosition(800, 0);
    leftSidebar.setFillColor(sf::Color(240, 240, 240)); // Light gray
    window.draw(leftSidebar);

    // Draw right sidebar background (move history area)
    sf::RectangleShape rightSidebar(sf::Vector2f(400, 800));
    rightSidebar.setPosition(1200, 0);
    rightSidebar.setFillColor(sf::Color(60, 60, 60)); // Dark grey
    window.draw(rightSidebar);


    // Draw move history 
    drawMoveHistory();

    // Draw captured pieces 
    drawCapturedPieces();



    // Draw resign button
    window.draw(resignButton);
    window.draw(resignButtonText);

    // Draw exit button
    window.draw(exitButton);
    window.draw(exitButtonText);
}

void ChessGUI::drawValidMoves() {
    const float circleRadius = 15.0f;
    const float captureRadius = 20.0f;
    
    for (const auto& move : validMoves) {
        float posX = move.x * 100.0f + 50.0f;
        float posY = (7 - move.y) * 100.0f + 50.0f;

        if (move.isCapture) {
            sf::CircleShape indicator(captureRadius);
            indicator.setFillColor(sf::Color(255, 0, 0, 80));
            indicator.setOutlineThickness(2);
            indicator.setOutlineColor(sf::Color::Red);
            indicator.setOrigin(captureRadius, captureRadius);
            indicator.setPosition(posX, posY);
            window.draw(indicator);
        } else {
            sf::CircleShape indicator(circleRadius);
            indicator.setFillColor(sf::Color(0, 255, 255, 80));  // Cyan with transparency
            indicator.setOutlineColor(sf::Color(0, 0, 255));     // Blue outline
            indicator.setOutlineThickness(2);
            indicator.setOrigin(circleRadius, circleRadius);
            indicator.setPosition(posX, posY);
            window.draw(indicator);
        }
    }
}

void Board::setGUI(ChessGUI* guiRef) { 
    gui = guiRef; 
}

bool Board::executeMove(int fromX, int fromY, int toX, int toY) {
    Piece fromPiece = squares[fromY][fromX];
    Piece toPiece = squares[toY][toX];

    if (!isValidMove(fromX, fromY, toX, toY)) {
        return false;
    }

    // Track captured pieces
    bool isCapture = false;
    bool isEnPassant = false;

    // Handle en passant capture
    if (fromPiece.type == Piece::PAWN && (toX != fromX) && squares[toY][toX].type == Piece::EMPTY) {
        int capturedPawnY = (fromPiece.color == Piece::WHITE) ? toY - 1 : toY + 1;
        toPiece = squares[capturedPawnY][toX];
        squares[capturedPawnY][toX] = Piece();
        isCapture = true;
        isEnPassant = true;
    }
    // Regular capture
    else if (squares[toY][toX].type != Piece::EMPTY) {
        isCapture = true;
    }

    // Record state BEFORE move
    Move currentMove;
    currentMove.prevHasKingMoved[0] = hasKingMoved[0];
    currentMove.prevHasKingMoved[1] = hasKingMoved[1];
    currentMove.prevHasRookMoved[0][0] = hasRookMoved[0][0];
    currentMove.prevHasRookMoved[0][1] = hasRookMoved[0][1];
    currentMove.prevHasRookMoved[1][0] = hasRookMoved[1][0];
    currentMove.prevHasRookMoved[1][1] = hasRookMoved[1][1];
    currentMove.prevKingX[0] = kingX[0];
    currentMove.prevKingX[1] = kingX[1];
    currentMove.prevKingY[0] = kingY[0];
    currentMove.prevKingY[1] = kingY[1];

    // Update king position FIRST if moving king
    if (fromPiece.type == Piece::KING) {
        int colorIndex = (fromPiece.color == Piece::WHITE) ? 0 : 1;
        kingX[colorIndex] = toX;
        kingY[colorIndex] = toY;
        hasKingMoved[colorIndex] = true;
    }

    // Perform the actual move
    squares[toY][toX] = fromPiece;
    squares[fromY][fromX] = Piece();

    // Handle castling
    if (fromPiece.type == Piece::KING && abs(toX - fromX) == 2) {
        int rookFromX = (toX == 6) ? 7 : 0;
        int rookToX = (toX == 6) ? 5 : 3;
        int y = fromY;

        // Move rook
        Piece rook = squares[y][rookFromX];
        squares[y][rookToX] = rook;
        squares[y][rookFromX] = Piece();

        // Update rook moved status
        int colorIndex = (fromPiece.color == Piece::WHITE) ? 0 : 1;
        if (toX == 6) {
            hasRookMoved[colorIndex][1] = true; // Kingside
        } else {
            hasRookMoved[colorIndex][0] = true; // Queenside
        }
    }

    // Handle rook movement tracking
    if (fromPiece.type == Piece::ROOK) {
        int colorIndex = (fromPiece.color == Piece::WHITE) ? 0 : 1;
        if (fromY == (colorIndex == 0 ? 0 : 7)) { // Original rank
            if (fromX == 0) hasRookMoved[colorIndex][0] = true;
            if (fromX == 7) hasRookMoved[colorIndex][1] = true;
        }
    }

    // Update turn AFTER potential promotion check
    currentTurn = (currentTurn == Piece::WHITE) ? Piece::BLACK : Piece::WHITE;

    // Record move details
    currentMove.fromX = fromX;
    currentMove.fromY = fromY;
    currentMove.toX = toX;
    currentMove.toY = toY;
    currentMove.piece = fromPiece;
    currentMove.capturedPiece = toPiece;
    currentMove.isCapture = isCapture;
    currentMove.isCastle = (fromPiece.type == Piece::KING && abs(toX - fromX) == 2);
    currentMove.isEnPassant = isEnPassant;
    currentMove.isPromotion = (fromPiece.type == Piece::PAWN && 
                              ((fromPiece.color == Piece::WHITE && toY == 7) ||
                               (fromPiece.color == Piece::BLACK && toY == 0)));

    // Record POST-move state
    currentMove.postHasKingMoved[0] = hasKingMoved[0];
    currentMove.postHasKingMoved[1] = hasKingMoved[1];
    currentMove.postHasRookMoved[0][0] = hasRookMoved[0][0];
    currentMove.postHasRookMoved[0][1] = hasRookMoved[0][1];
    currentMove.postHasRookMoved[1][0] = hasRookMoved[1][0];
    currentMove.postHasRookMoved[1][1] = hasRookMoved[1][1];
    currentMove.postKingX[0] = kingX[0];
    currentMove.postKingX[1] = kingX[1];
    currentMove.postKingY[0] = kingY[0];
    currentMove.postKingY[1] = kingY[1];

    moveHistory.push_back(currentMove);
    return true;
}

bool Board::isCaptureMove(int fromX, int fromY, int toX, int toY) const {
    const Piece& fromPiece = squares[fromY][fromX];
    const Piece& toPiece = squares[toY][toX]; // Corrected from [toY][toY] to [toY][toX]

    if (fromPiece.type == Piece::PAWN && toX != fromX && toPiece.type == Piece::EMPTY) {
        // Check en passant
        return (lastmove.isTwoSquarePawnMove &&
                lastmove.piece.type == Piece::PAWN &&
                lastmove.toX == toX &&
                (lastmove.toY == toY + 1 || lastmove.toY == toY - 1));
    }

    return (toPiece.type != Piece::EMPTY && toPiece.color != fromPiece.color);
}

bool Board::canAttack(int fromX, int fromY, int toX, int toY) const {
    // Boundary checks
    if (fromX < 0 || fromX >= 8 || fromY < 0 || fromY >= 8 ||
        toX < 0 || toX >= 8 || toY < 0 || toY >= 8) return false;

    const Piece& fromPiece = squares[fromY][fromX];
    if (fromPiece.type == Piece::EMPTY) return false;

    int dx = abs(toX - fromX);
    int dy = abs(toY - fromY);

    // Check piece movement patterns
    switch (fromPiece.type) {
        case Piece::PAWN:
            if (fromPiece.color == Piece::WHITE) {
                return (dx == 1 && (toY - fromY == 1)); // White pawn attack
            } else {
                return (dx == 1 && (fromY - toY == 1)); // Black pawn attack
            }
            
        case Piece::KNIGHT:
            return (dx == 2 && dy == 1) || (dx == 1 && dy == 2);
            
        case Piece::BISHOP:
            return (dx == dy) && validBishopMove(fromX, fromY, toX, toY);
            
        case Piece::ROOK:
            return (dx == 0 || dy == 0) && validRookMove(fromX, fromY, toX, toY);
            
        case Piece::QUEEN:
            return ((dx == dy) || (dx == 0 || dy == 0)) && 
                   validQueenMove(fromX, fromY, toX, toY);
            
        case Piece::KING:
            return dx <= 1 && dy <= 1;
            
        default:
            return false;
    }
}

// Check if a move is valid
bool Board::isValidMove(int fromX, int fromY, int toX, int toY) const{
    // Boundary checks
    if (fromX < 0 || fromX >= 8 || fromY < 0 || fromY >= 8 ||
        toX < 0 || toX >= 8 || toY < 0 || toY >= 8) {
        return false;
    }


    Piece fromPiece = squares[fromY][fromX];
    Piece toPiece = squares[toY][toX];

    // Ensure the piece belongs to the current player
    if (fromPiece.color != currentTurn) {
        return false; // Not this player's turn
    }

    // Prevent capturing the opponent's king
    if (toPiece.type == Piece::KING) {
        return false;
    }

    // Can't move empty square or capture own color
    if (fromPiece.type == Piece::EMPTY) return false;
    if (toPiece.type != Piece::EMPTY && fromPiece.color == toPiece.color) return false;

    int dx = abs(toX - fromX);
    int dy = toY - fromY; // Direction matters for pawns

    bool validMove = false; // Track if move matches piece rules

    switch (fromPiece.type) {
        case Piece::PAWN:
            // White pawn moves
            if (fromPiece.color == Piece::WHITE) {
                // Forward one
                if (dx == 0 && dy == 1 && squares[toY][toX].type == Piece::EMPTY) validMove = true;
                // Forward two (initial)
                else if (dx == 0 && dy == 2 && fromY == 1 && 
                         squares[2][fromX].type == Piece::EMPTY && 
                         squares[3][fromX].type == Piece::EMPTY &&
                         squares[toY][toX].type == Piece::EMPTY) validMove = true;
                // Capture
                else if (dx == 1 && dy == 1 && 
                         ((toPiece.color == Piece::BLACK &&
                            toPiece.type != Piece::EMPTY)|| isEnPassantCaptureValid(fromX, fromY, toX, toY, fromPiece))) {
                    validMove = true;
                }
            } 
            // Black pawn moves
            else {
                // Forward one
                if (dx == 0 && dy == -1 && squares[toY][toX].type == Piece::EMPTY) validMove = true;
                // Forward two (initial)
                else if (dx == 0 && dy == -2 && fromY == 6 && 
                         squares[5][fromX].type == Piece::EMPTY && 
                         squares[4][fromX].type == Piece::EMPTY &&
                         squares[toY][toX].type == Piece::EMPTY) validMove = true;
                // Capture
                else if (dx == 1 && dy == -1 && 
                         ((toPiece.color == Piece::WHITE && 
                           toPiece.type != Piece::EMPTY) || isEnPassantCaptureValid(fromX, fromY, toX, toY, fromPiece))) {
                    validMove = true;
                }
            }
            break;

        case Piece::KNIGHT:
            validMove = (dx == 2 && abs(dy) == 1) || (dx == 1 && abs(dy) == 2);
            break;

        case Piece::BISHOP:
            validMove = (dx == abs(dy)) && validBishopMove(fromX, fromY, toX, toY);
            break;

        case Piece::ROOK:
            validMove = (dx == 0 || dy == 0) && validRookMove(fromX, fromY, toX, toY);
            break;

        case Piece::QUEEN:
            validMove = (dx == 0 || dy == 0 || dx == abs(dy)) && validQueenMove(fromX, fromY, toX, toY);
            break;

        case Piece::KING:
            validMove = (
                (dx <= 1 && abs(dy) <= 1 && 
                !isSquareUnderAttack(toX, toY, fromPiece.color)
            ) || (
                (dx == 2 && dy == 0 &&
                isCastleValid(toX, fromPiece.color)
            )));
            break;

        default:
            validMove = false;
    }

    if (!validMove) return false; // Reject invalid moves early

    // // Simulate move to check king safety
    // squares[toY][toX] = fromPiece;
    // squares[fromY][fromX] = Piece();

    // // Temporarily update king position if moving the king
    // int originalKingX = -1, originalKingY = -1;
    // if (fromPiece.type == Piece::KING) {
    //     int colorIndex = (fromPiece.color == Piece::WHITE) ? 0 : 1;
    //     originalKingX = kingX[colorIndex];
    //     originalKingY = kingY[colorIndex];
    //     kingX[colorIndex] = toX;
    //     kingY[colorIndex] = toY;
    // }

    // bool inCheck = isKingInCheck(fromPiece.color);

    // // Undo simulation
    // squares[fromY][fromX] = fromPiece;
    // squares[toY][toX] = toPiece;

    // // Restore king position if moved
    // if (fromPiece.type == Piece::KING) {
    //     int colorIndex = (fromPiece.color == Piece::WHITE) ? 0 : 1;
    //     kingX[colorIndex] = originalKingX;
    //     kingY[colorIndex] = originalKingY;
    // }

    // return !inCheck; // Only allow move if king isn't in check

     // Simulate move
    Board tempBoard = *this;
    tempBoard.squares[toY][toX] = fromPiece;
    tempBoard.squares[fromY][fromX] = Piece();
    
    // Update temp board's king position
    if (fromPiece.type == Piece::KING) {
        int idx = (fromPiece.color == Piece::WHITE) ? 0 : 1;
        tempBoard.kingX[idx] = toX;
        tempBoard.kingY[idx] = toY;
    }
    
    return !tempBoard.isKingInCheck(fromPiece.color);
}

bool Board::validBishopMove(int fromX, int fromY, int toX, int toY)const{
        
    bool flag = true;
    if(fromX < toX && fromY < toY){
        for(int i = fromX+1, j = fromY+1; i<toX && j<toY; i++, j++){
            if(squares[j][i].type != Piece :: EMPTY){
                flag = false;
                break;
            }
        }
    }
    else if(fromX > toX && fromY > toY){
        for(int i = fromX-1, j = fromY-1; i>toX && j>toY; i--, j--){
            if(squares[j][i].type != Piece :: EMPTY){
                flag = false;
                break;
            }
        }
    }
    else if(fromX > toX && fromY < toY){
        for(int i = fromX-1, j = fromY+1; i>toX && j<toY; i--, j++){
            if(squares[j][i].type != Piece :: EMPTY){
                flag = false;
                break;
            }
        }
    }
    else if(fromX < toX && fromY > toY){
        for(int i = fromX+1, j = fromY-1; i<toX && j>toY; i++, j--){
            if(squares[j][i].type != Piece :: EMPTY){
                flag = false;
                break;
            }
        }
    }

    return flag;
}

bool Board::validRookMove(int fromX, int fromY, int toX, int toY)const{
    
    bool flag = true;
    if(fromY == toY){
        if(fromX < toX){
            for(int i = fromX+1; i< toX; i++){
                if(squares[toY][i].type != Piece :: EMPTY){
                    flag = false;
                    break;
                }
            }
        }
        else if(fromX > toX){
            for(int i = fromX-1; i> toX; i--){
                if(squares[toY][i].type != Piece :: EMPTY){
                    flag = false;
                    break;
                }
            }
        }
    }
    else if(fromX == toX){
        if(fromY < toY){
            for(int i = fromY+1; i< toY; i++){
                if(squares[i][toX].type != Piece :: EMPTY){
                    flag = false;
                    break;
                }
            }
        }
        else if(fromY > toY){
            for(int i = fromY-1; i> toY; i--){
                if(squares[i][toX].type != Piece :: EMPTY){
                    flag = false;
                    break;
                }
            }
        }
    }
    

    return flag;
}

bool Board::validQueenMove(int fromX, int fromY, int toX, int toY)const{

    if(validBishopMove(fromX, fromY, toX, toY) && validRookMove(fromX, fromY, toX, toY)){
        return true;
    }
    else{
        return false;
    }
}

bool Board::hasQueen(Piece::Color color) const {
    for(int y=0; y<8; y++) 
        for(int x=0; x<8; x++)
            if(squares[y][x].type == Piece::QUEEN && 
               squares[y][x].color == color) return true;
    return false;
}

void Board::performCastle(int toX, Piece::Color color) {
    int y = (color == Piece::WHITE) ? 0 : 7;
    if (toX == 6) { // Kingside
        squares[y][5] = squares[y][7];
        squares[y][7] = Piece();
        hasRookMoved[color == Piece::WHITE ? 0 : 1][1] = true;
    } else { // Queenside
        squares[y][3] = squares[y][0];
        squares[y][0] = Piece();
        hasRookMoved[color == Piece::WHITE ? 0 : 1][0] = true;
    }
    hasKingMoved[color == Piece::WHITE ? 0 : 1] = true;
}


std::string ChessGUI::moveToAlgebraic(const Board::Move& move) {
    std::string notation;

    // Castling
    if (move.isCastle) {
        return (move.toX == 6) ? "O-O" : "O-O-O";
    }

    // Destination square
    char file = 'a' + move.toX;
    char rank = '1' + move.toY;
    std::string dest = std::string(1, file) + rank;

    // Piece symbol
    char pieceChar = ' ';
    switch (move.piece.type) {
        case Piece::KNIGHT: pieceChar = 'N'; break;
        case Piece::BISHOP: pieceChar = 'B'; break;
        case Piece::ROOK: pieceChar = 'R'; break;
        case Piece::QUEEN: pieceChar = 'Q'; break;
        case Piece::KING: pieceChar = 'K'; break;
        default: break;
    }

    // Pawn handling
    bool isPawn = (move.piece.type == Piece::PAWN);
    if (isPawn && (move.isCapture || move.isEnPassant)) {
        notation += ('a' + move.fromX);
    } else if (!isPawn) {
        notation += pieceChar;
    }

    // Capture
    if (move.isCapture || move.isEnPassant) {
        notation += 'x';
    }

    notation += dest;

    // Promotion
    if (move.isPromotion) {
        char promo = 'Q';
        switch (move.promotionType) {
            case Piece::ROOK: promo = 'R'; break;
            case Piece::BISHOP: promo = 'B'; break;
            case Piece::KNIGHT: promo = 'N'; break;
            default: break;
        }
        notation += "=";
        notation += promo;
    }

    return notation;
}


void ChessGUI::render() {
    window.clear(sf::Color::White);
    
    // Draw chessboard
    window.draw(boardSprite);
    
    // Draw piece highlights
    drawHighlight();

    // Draw all pieces except dragged one
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            // Skip drawing the dragged piece's original position
            if (isDragging && x == selectedX && y == selectedY) continue;
            
            sf::Sprite pieceSprite = createBoardPieceSprite(x, y);
            if (pieceSprite.getTexture()) {
                window.draw(pieceSprite);
            }
        }
    }

    // Draw legal move indicators
    drawValidMoves();

    // After drawing pieces, before dragged piece
    drawCheckIndicator();

    // Draw dragged piece on top of everything
    if (isDragging) {
        window.draw(draggedPiece);
    }

    if (isPromoting) {
        drawPromotionMenu();
    }

    // Game result
    if(gameOver) {
        // Dark overlay
        window.draw(gameOverlay);
        
        // Dialog box
        window.draw(gameResultDialog);
        window.draw(gameResultText);
        window.draw(newGameButton);
        window.draw(newGameButtonText);
        window.draw(exitGameButton);
        window.draw(exitGameButtonText);
    }

    // Draw resign dialog if open
    if (isResignDialogOpen) {
        // Darken background
        sf::RectangleShape overlay(sf::Vector2f(1200, 800));
        overlay.setFillColor(sf::Color(0, 0, 0, 150));
        window.draw(overlay);

        // Dialog box
        window.draw(resignDialog);
        window.draw(resignDialogText);
        window.draw(resignDialogYesButton);
        window.draw(resignDialogYesText);
        window.draw(resignDialogNoButton);
        window.draw(resignDialogNoText);
    }

    // Draw sidebar UI elements
    drawSidePanel();

    // Display AI thinking status
    if (aiThinking) {
        sf::Text thinkingText("AI is thinking...", font, 24);
        thinkingText.setPosition(810, 50);
        thinkingText.setFillColor(sf::Color::Red);
        // window.draw(thinkingText);
    }

    // Draw game state text
    window.draw(turnText);
    window.draw(result);
    window.draw(undoButton);
    window.draw(undoButtonText);
    window.draw(redoButton);
    window.draw(redoButtonText);

    window.display();
}


int main() {
    while (true) {
        StartScreen startScreen;
        startScreen.run();

        if (!startScreen.isPlayClicked()) {
            break; // Exit if the user closed the window or clicked exit
        }

        // Show difficulty selection
        DifficultyScreen diffScreen;
        diffScreen.run();
        int difficulty = diffScreen.getSelectedDifficulty();

        if (difficulty == -1) {
            continue; // User went back, restart the loop
        }

        // Start the game with the selected difficulty
        ChessGUI chessGame(difficulty);
        chessGame.run();
    }

    return 0;
}

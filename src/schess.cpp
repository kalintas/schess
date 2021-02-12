#include "schess.h"

#include "renderer.h"
#include "game_logic.h"

#include <GLFW/glfw3.h>

#include <chrono> // // //

namespace schess
{

    static vec2<int> s_ScreenSize{ 720, 720 };

    static GLFWwindow* s_window;
    static gl::shader s_shader;
    
    static gameLogic s_game;

    static const size2<float> s_texPieceSize{ 1.0f / 7.0f, 1.0f / 2.0f };

    static size2<float> s_fBoardSize;
    static size2<float> s_fBoardGridSize;
    static vec2<float>  s_fBoardPos;

    static size2<int> s_boardSize;
    static size2<int> s_boardGridSize;
    static vec2<int>  s_boardPos;

    static inline vec2<int> s_getScreenLocation(const vec2<float> normalized)
    {
        const vec2<float> temp{ (normalized + vec2<float>{ 1.0f, 1.0f }) * 0.5f };

        return static_cast<vec2<int>>(vec2<float>{ temp.x, 1.0f - temp.y } * static_cast<vec2<float>>(s_ScreenSize));
    }

    static inline size2<int> s_getScreenSizeConverted(const vec2<float> normalized)
    {
        return static_cast<vec2<int>>(normalized * 0.5f * static_cast<vec2<float>>(s_ScreenSize));
    }

    void ResetGame()
    {
        s_fBoardSize     = { 1.6f, 1.6f };
        s_fBoardGridSize = s_fBoardSize / 8.0f;
        s_fBoardPos      = { -0.8f, 0.8f };

        s_boardSize     = s_getScreenSizeConverted(s_fBoardSize);
        s_boardGridSize = s_getScreenSizeConverted(s_fBoardGridSize);
        s_boardPos      = s_getScreenLocation(s_fBoardPos);
        
    }

    static void s_glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        using namespace std::chrono;

        static auto s_undoTimer = high_resolution_clock::now();

        switch(key)
        {
        case GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose(window, true);
        break;
        case GLFW_KEY_Z:

        const auto elapsed = duration_cast<duration<float>>(high_resolution_clock::now() - s_undoTimer);

        if(elapsed.count() > 0.1f) 
        {
            s_game.m_undoMove();
            s_undoTimer = high_resolution_clock::now();
        }
        break;
        }
    }

    static vec2<int> s_getMousePos()
    {
        vec2<double> pos;
        glfwGetCursorPos(s_window, &pos.x, &pos.y);
        
        return static_cast<vec2<int>>(pos);
    }

    static int s_selecetedPieceIndex;
    static bool s_pieceSelected = false;

    std::vector<int> s_selectedPieceMovements;

    static auto s_inversePieceColors = gl::s_getUniform<1, int>(glUniform1i, vec4<int>{ false });

    static void s_fillSelectedPieceMovements()
    {   
        for(int i = 0; i < 64; ++i)
        {
            if(s_game.m_isMoveValid(s_selecetedPieceIndex, i)) 
            {
                s_selectedPieceMovements.push_back(i);
            }
        }   
    }
    
    static bool s_selectedCanMove(const int index)
    {

        for(const auto val : s_selectedPieceMovements)
        {
            if(val == index) return true;
        }

        return false;
    }

    static void s_glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
        if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        {   
            const vec2<int> mousePos{ s_getMousePos() - s_boardPos };

            if(!mousePos.m_isInRange({ 0, 0 }, s_boardSize)) { s_pieceSelected = false; return; }

            const int index = gameLogic::s_getIndex(mousePos / s_boardGridSize);

            if(!s_pieceSelected)
            {
                if(!s_game.m_isSelectable(index)) return;

                s_selecetedPieceIndex = index;
                
                s_fillSelectedPieceMovements();
            }
            else 
            { 
                if(s_game.m_playIfValid(s_selecetedPieceIndex,  index)) s_game.m_playEnemy(); 

                s_selectedPieceMovements.clear();    
            }

            s_pieceSelected = !s_pieceSelected;
        }

    }

    // TODO
    // animation when piece moves

    bool Init()
    {
        ASSERT(glfwInit() == GLFW_TRUE, "cannot init glfw");

        s_window = glfwCreateWindow(s_ScreenSize.x, s_ScreenSize.y, "simple chess", nullptr, nullptr);

        ASSERT(s_window, "cannot create a window");
            
        glfwMakeContextCurrent(s_window);

        //open vsync
        glfwSwapInterval(1);

        // // call back functions for getting events
        glfwSetMouseButtonCallback(s_window, s_glfwMouseButtonCallback);
        glfwSetKeyCallback        (s_window, s_glfwKeyCallback        );

        ASSERT(glewInit() == GLEW_OK, "cannot init glew");

        ASSERT(
        s_shader.m_createShaders("../res/vertex.glsl", "../res/fragment.glsl"), 
        "cannot create shaders");

        s_shader.m_bind();
        
        GLCALL(glEnable(GL_BLEND));
        GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        // CreateTextures("../res/board.png", "../res/pieces.png", s_shader);
        // ResetGame();
        ResetGame();

        s_inversePieceColors.m_create(s_shader, "u_bInverseTextureColors");

        return true;
    }

    template<bool inverseColors = false>
    static void s_drawPiece(gl::texture2DRenderer& pieceTexture, const int index, const vec2<float> textureVec)
    {
        const vec2<float> currScreenPos{ static_cast<float>(index % 8), static_cast<float>(index / 8) };

        const vec2<float> screenVec = s_fBoardGridSize * currScreenPos;
        
        pieceTexture.m_setBuffer(
        { { s_fBoardPos.x + screenVec.x, s_fBoardPos.y - screenVec.y} , s_fBoardGridSize }, 
        { s_texPieceSize * textureVec, s_texPieceSize });

        if constexpr(inverseColors) s_inversePieceColors.setVec(vec4<int>{ true });

        pieceTexture.m_draw();

        if constexpr(inverseColors) s_inversePieceColors.setVec(vec4<int>{ false });
    }

    void Run()
    {   
        {
        gl::texture2DRenderer boardTexture("../res/board.png" , s_shader, { GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT });
        gl::texture2DRenderer pieceTexture("../res/piecesDenem04.png", s_shader, { GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE });

        boardTexture.m_setBuffer(
            { s_fBoardPos.x, s_fBoardPos.y, s_fBoardSize.x, s_fBoardSize.y }, 
            { 0.0f, 4.0f, 4.0f, 4.0f });

        while(!glfwWindowShouldClose(s_window))
        {
            glfwPollEvents();            

            //clear screen
            GLCALL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
            GLCALL(glClear(GL_COLOR_BUFFER_BIT));

            boardTexture.m_draw();

            for(int i = 0; i < 64; ++i)
            {  
                const boardGrid current = s_game.m_getAt(i);
                const bool canMove = s_selectedCanMove(i);

                if(current.m_isEmpty() && !canMove) continue;

                if(s_pieceSelected && s_selecetedPieceIndex == i) s_drawPiece<true>(pieceTexture, i, current.m_getVec());
                else                                              s_drawPiece      (pieceTexture, i, current.m_getVec());

                if(canMove) s_drawPiece(pieceTexture, i, { 6.0f, 2.0f });            
            } 

            glfwSwapBuffers(s_window);
        }
        
        }
        // DeleteTextures();
        glfwTerminate();
    }
}

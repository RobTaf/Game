#ifndef BaseAnimation_hpp
#define BaseAnimation_hpp

#include <sstream>

/// @brief A frame is a single image in a sequence of images
/// that make up the animation. Each frame is represented as
/// an index.
using Frame = unsigned int;
class SpriteSheet;

/// @brief Base class of an animation. For now, only one type
/// of animation is implemented: directional animation. This type
/// of animation is usually used with any kind of moving entities
/// that have a specific animation for each direction. Unlike other
/// types of animations, where an increasing frame can lead to a jump 
/// in rows, directional animation will always remain on the row that
/// represents the proper type of animation in the proper direction.
class BaseAnimation {
    friend class SpriteSheet;
public:
    BaseAnimation();
    virtual ~BaseAnimation();

    void play();
    void pause();
    void stop();
    void reset();
    bool isInAction();

    virtual void update(const float& deltaTime);

    friend std::stringstream& operator >> (std::stringstream& stream, BaseAnimation& animation) {
        animation.readIn(stream);
        return stream;
    }

    void setSpriteSheet(SpriteSheet* sheet);
    SpriteSheet* getSpriteSheet();

    void setName(const std::string& name);
    std::string getName();

    void setFrame(const Frame& frame);
    Frame getFrame();

    void setStartFrame(const Frame& frame);
    Frame getStartFrame();

    void setEndFrame(const Frame& frame);
    Frame getEndFrame();

    void setRowFrame(const Frame& frame);
    Frame getRowFrame();

    void setStartActionFrame(int& frame);
    int getStartActionFrame();

    void setEndActionFrame(int& frame);
    int getEndActionFrame();

    void setTimeFrame(float& timeFrame);
    float getTimeFrame();

    void setElapsedTime(float& elapsedTime);
    float getElapsedTime();

    void setLoop(const bool& loop);
    bool isLooping();

    void setPlaying(const bool& playing);
    bool isPlaying();

protected:
    virtual void frameStep() = 0;
    virtual void cropSprite() = 0;
    virtual void readIn(std::stringstream& stream) = 0;

    /// @brief Represents the current frame of the animation
    /// being displayed. This frame changes as the animation
    /// progresses.
    Frame m_currentFrame;
    /// @brief Represents the first frame of the animation or
    /// the starting point in the sequence. It is used to reset
    /// or loop the animation from the beginning.
    Frame m_startFrame;
    /// @brief Represents the last frame of the animation. This
    /// is used to know when the animation should stop or loop
    /// back to the first frame.
    Frame m_endFrame;
    /// @brief This represents the frame within a particular row
    /// in the sprite sheet. It is useful for animations that span
    /// multiple rows in the sprite sheet.
    Frame m_rowFrame;
    /// @brief Represents the starting frame for a specific action
    /// that should happen during the animation. If it set to -1,
    /// the behaviour can happen at any point in the animation.
    /// It is used for triggering in-game actions like attacks or
    /// effects at a specific point in the animation.
    int m_actionStartFrame;
    /// @brief Represents the ending frame for the action tied to
    /// the start action frame. The action is active in [m_actionStartFrame, m_actionEndFrame]
    /// If it set to -1, the action can extend for the entire duration 
    /// of the animation.
    int m_actionEndFrame;
    /// @brief Represents the duration each frame in the animation should be
    /// displayed for. This controls the speed of the animation by determining
    /// how long each frame stays visible before switching to the next one.
    float m_timeFrame;
    /// @brief Keeps track of the amount of time that has passed since the
    /// animation started or since the last frame change. This is used when
    /// to transition to the next frame based on the frame time.
    float m_elapsedTime;
    /// @brief Flag indicating wether the animation should loop.
    bool m_loop;
    /// @brief Flag indicating wether the animation is currently playing or not.
    /// If true, the animation is active and progressing through frames. If false,
    /// the animation is paused or stopped.
    bool m_playing;
    /// @brief Represents the name of the animation.
    std::string m_name;
    /// @brief Pointer to the SpriteSheet class.
    SpriteSheet* m_spriteSheet;
};

#endif
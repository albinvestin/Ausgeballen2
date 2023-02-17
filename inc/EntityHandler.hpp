#ifndef ENTITYHANDLER_HPP
#define ENTITYHANDLER_HPP
#include "Entities.hpp"
#include "Constants.hpp"
#include <vector>
// TODO: You should not be #includeing at all. Since you only store pointers, you do not need the full definition of the classes - you only need the declaration. Simply use the forward declarations by themselves

// Forward delcaration:
// class NetworkHandler;


class EntityHandler
{
private:
    // Player _P1;
    // Player _P2;
    std::vector<Player> _players; // Make references to players instead of pass by value.
    std::vector<Bullet> _existingBullets;
    uint16_t _nextBulletID = 0;
public:
    EntityHandler();
    ~EntityHandler();
    void HandlePlayerActions(const GAMELOOP_ACTIONS &actions);
    // Get methods
    Vec2f GetPlayerPos(int index) const; // Index starts at 1
    float GetPlayerAim(int index) const;
    uint8_t GetPlayerScore(int index) const;
    Vec2f GetBullet1Pos() const;
    std::vector<Vec2f> GetAllBulletPos() const;
    const std::vector<Bullet> &GetAllBullets() const;
    std::vector<Player> &GetAllPlayers(); // TODO This is bad?
    GameSnapshot GetGameSnapShot() const;

    // Update methods
    Vec2f UpdatePlayerPos(uint8_t playerIndex);
    Vec2f AddRecoil(uint8_t playerIndex);
    void SetRecoilOfPlayer(const Vec2f &vel, uint8_t playerIndex);
    // const Bullet& RegisterNewBullet(const Bullet& bullet);
    // void AddNetworkBullet(Bullet& b);
    float UpdateAimDirection(uint8_t playerIndex);
    Vec2f UpdateBulletPos(std::vector<Bullet>::iterator bullet) const;
    std::vector<Bullet>::const_iterator RemoveBulletFromIt(std::vector<Bullet>::const_iterator itB);
    void HandleNetworkShoot(const Bullet &b, const Vec2f &v, uint8_t playerIndex, const Vec2f &playerPos);
    void HandleNetworkGameSnapshot(const GameSnapshot& gs);
    void MoveAllObjects();
};
#endif /* ENTITYHANDLER_HPP */

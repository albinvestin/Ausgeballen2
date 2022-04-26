#ifndef ENTITYHANDLER_HPP
#define ENTITYHANDLER_HPP
// #include "Player.hpp"
#include "Entities.hpp"
// #include "NetworkHandler.hpp"
#include <vector>
// TODO: You should not be #includeing at all. Since you only store pointers, you do not need the full definition of the classes - you only need the declaration. Simply use the forward declarations by themselves

// Forward delcaration:
class NetworkHandler;


class EntityHandler
{
private:
    // Player _P1;
    // Player _P2;
    std::vector<Player> _players; // Make references to players instead of pass by value.
    std::vector<Bullet> _existingBullets;
    NetworkHandler* _networkHandler = NULL;
    uint16_t _nextBulletID = 0;
public:
    EntityHandler(NetworkHandler* networkHandler);
    ~EntityHandler();
    void ServerCheckAndHandleShoot(int inputkeys);
    Vec2f GetPlayerPos(int index) const; // Index starts at 1
    float GetPlayerAim(int index) const;
    int GetPlayerScore(int index) const;
    Vec2f GetBullet1Pos();
    std::vector<Vec2f> GetAllBulletPos() const;
    const std::vector<Bullet>& GetAllBullets();
    std::vector<Player>& GetAllPlayers(); // TODO This is bad?
    Vec2f UpdatePlayerPos(uint8_t playerIndex);
    Vec2f AddRecoil(uint8_t playerIndex);
    void SetRecoilOfPlayer(Vec2f vel, uint8_t playerIndex);
    // const Bullet& RegisterNewBullet(const Bullet& bullet);
    // void AddNetworkBullet(Bullet& b);
    float UpdateAimDirection(uint8_t playerIndex);
    Vec2f UpdateBulletPos(std::vector<Bullet>::iterator bullet);
    std::vector<Bullet>::const_iterator RemoveBulletFromIt(std::vector<Bullet>::const_iterator itB);
    void HandleNetworkShoot(Bullet& b, Vec2f& v, uint8_t playerIndex, Vec2f& playerPos);
    void HandleNetworkGameSnapshot(const GameSnapshot& gs);
    void MoveAllObjects();
    void UpdateClients();
};
#endif /* ENTITYHANDLER_HPP */

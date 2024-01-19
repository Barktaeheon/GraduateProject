#include "PickingManager.h"
#include "GridObject.h"

PickingManager::PickingManager()
{
    m_pMousePos = ImVec2(0.f, 0.f);
}

PickingManager::~PickingManager()
{
}

void PickingManager::CalculateScreenMouseCoords()
{
    m_pMousePos = ImGui::GetMousePos();
}

Ray PickingManager::CreateWorldRayFromScreenCoords(float screenX, float screenY, const shared_ptr<CCamera>& pCamera)
{
    XMFLOAT4X4 viewMatrixBeforeLoad = pCamera->GetViewMatrix();
    XMFLOAT4X4 projectionMatrixBeforeLoad = pCamera->GetProjectionMatrix();

    XMMATRIX viewMatrix = XMLoadFloat4x4(&viewMatrixBeforeLoad);
    XMMATRIX invViewMatrix = XMMatrixInverse(nullptr, viewMatrix);

    XMFLOAT3 CameraPositionfloat3;
    XMStoreFloat3(&CameraPositionfloat3, invViewMatrix.r[3]);

    XMVECTOR CameraPositionVector = XMLoadFloat3(&CameraPositionfloat3);

    XMMATRIX projectionMatrix = XMLoadFloat4x4(&projectionMatrixBeforeLoad);
    XMMATRIX viewProjectionInverse = XMMatrixInverse(nullptr, viewMatrix * projectionMatrix);

    XMMATRIX viewProjectionMatrix = viewMatrix * projectionMatrix;

    float clipX = (2.0f * screenX / pCamera->GetViewPort().Width - 1.0f);
    float clipY = (1.0f - 2.0f * screenY / pCamera->GetViewPort().Height);

    XMVECTOR clipSpacePos = XMVectorSet(clipX, clipY, 1.0f, 1.0f);
    XMVECTOR worldSpacePos = XMVector4Transform(clipSpacePos, viewProjectionInverse);
    worldSpacePos /= XMVectorGetW(worldSpacePos);

    XMFLOAT3 worldSpace;
    XMStoreFloat3(&worldSpace, worldSpacePos);

    //���� ����
    Ray ray;
    ray.origin = CameraPositionVector;
    XMFLOAT3 rayDirection;

    rayDirection.x = worldSpace.x - CameraPositionfloat3.x;
    rayDirection.y = worldSpace.y - CameraPositionfloat3.y;
    rayDirection.z = worldSpace.z - CameraPositionfloat3.z;

    XMVECTOR normalizedrayDirection = XMVector3Normalize(XMLoadFloat3(&rayDirection));
    ray.direction = normalizedrayDirection;
    return ray;
}

RayHitResult PickingManager::ReturnScreenToWorldRayClosestHitResult(Ray pRay, const shared_ptr<CScene>& pScene, const shared_ptr<CCamera>& pCamera)
{
    RayHitResult closestHitResult;
    vector<shared_ptr<CGameObject>> sceneObjects = pScene->GetObjects();

    for (const auto& sceneObject : sceneObjects)
    {
        RayHitResult currentHitResult = RayIntersectionWithWorldObject(pRay, sceneObject, sceneObject->GetWorld4X4());

        //�������� �ִ� �Ÿ��� HitResult��ȯ
        if (currentHitResult.hit && (!closestHitResult.hit || currentHitResult.hitDistance < closestHitResult.hitDistance))
        {
            closestHitResult = currentHitResult;
        }
    }

    return closestHitResult;
}

RayHitResult PickingManager::RayIntersectionWithWorldObject(const Ray& ray, const shared_ptr<CGameObject>& Object, const XMFLOAT4X4& worldMatrix)
{
    RayHitResult result;
    shared_ptr<CMesh> ObjectMesh = Object->GetMesh();

    // ���̿� �ٿ�� �ڽ� �浹 �˻�
    float hitDistance;
    BoundingOrientedBox objectOrientedBox = ObjectMesh->GetBoundingBox();

    if (!objectOrientedBox.Intersects(ray.origin, ray.direction, hitDistance))
        return result;

    for (size_t j = 0; j < ObjectMesh->GetSubSetIndices().size(); j++)
    {
        for (size_t k = 0; k + 2 < ObjectMesh->GetSubSetIndices()[j].size(); k += 3)
        {
            XMFLOAT3 vertex0 = Vector3::TransformCoord(ObjectMesh->GetPositions()[ObjectMesh->GetSubSetIndices()[j][k]], worldMatrix);
            XMFLOAT3 vertex1 = Vector3::TransformCoord(ObjectMesh->GetPositions()[ObjectMesh->GetSubSetIndices()[j][k + 1]], worldMatrix);
            XMFLOAT3 vertex2 = Vector3::TransformCoord(ObjectMesh->GetPositions()[ObjectMesh->GetSubSetIndices()[j][k + 2]], worldMatrix);

            float distance;
            if (DirectX::TriangleTests::Intersects(ray.origin, ray.direction,
                XMLoadFloat3(&vertex0), XMLoadFloat3(&vertex1), XMLoadFloat3(&vertex2), distance))
            {
                result.hit = true;
                result.hitObject = Object;
                //���� �ͺ��� �տ� �ִ��� �˻�
                if (distance < result.hitDistance)
                {
                    XMVECTOR intersectionPointVector = XMVectorAdd(ray.origin, XMVectorScale(ray.direction, distance));
                    XMFLOAT3 intersectionPoint;
                    XMStoreFloat3(&intersectionPoint, intersectionPointVector);

                    result.hitPosition = intersectionPoint;
                    result.hitDistance = distance;
                }
            }
        }
    }
    return result;
}

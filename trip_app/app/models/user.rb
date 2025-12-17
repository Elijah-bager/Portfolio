class User < ApplicationRecord
  # Include default devise modules. Others available are:
  # :confirmable, :lockable, :timeoutable, :trackable and :omniauthable
  devise :database_authenticatable, :registerable,
         :recoverable, :rememberable, :validatable
  has_many :trips, foreign_key: :host_id
  has_many :user_trips
  has_many :part_trips, through: :user_trips, source: :trip
  has_many :expenses, foreign_key: :spender_id
  has_many :debts, foreign_key: :debtor_id
  validates :name, uniqueness: true
end
